const API_BASE = '';

let currentQuery = '';
let suggestions = [];
let selectedSuggestionIndex = -1;
let suggestionTimeout = null;

// Initialize
document.addEventListener('DOMContentLoaded', () => {
    const searchInput = document.getElementById('search-input');
    const searchBtn = document.getElementById('search-btn');
    const clearBtn = document.getElementById('clear-btn');
    
    // Check server status
    checkStatus();
    
    // Auto-suggestions on input
    searchInput.addEventListener('input', (e) => {
        const query = e.target.value.trim();
        
        if (query) {
            clearBtn.style.display = 'block';
            // Debounce suggestions
            clearTimeout(suggestionTimeout);
            suggestionTimeout = setTimeout(() => {
                fetchSuggestions(query);
            }, 200);
        } else {
            clearBtn.style.display = 'none';
            hideSuggestions();
        }
    });
    
    // Search on Enter key
    searchInput.addEventListener('keydown', (e) => {
        const suggestionsContainer = document.getElementById('suggestions-container');
        const suggestionsList = document.getElementById('suggestions-list');
        
        if (e.key === 'Enter') {
            if (selectedSuggestionIndex >= 0 && suggestions.length > 0) {
                e.preventDefault();
                searchInput.value = suggestions[selectedSuggestionIndex];
                hideSuggestions();
                performSearch();
            } else {
                performSearch();
            }
        } else if (e.key === 'ArrowDown') {
            e.preventDefault();
            if (suggestionsContainer.style.display !== 'none' && suggestions.length > 0) {
                selectedSuggestionIndex = Math.min(selectedSuggestionIndex + 1, suggestions.length - 1);
                updateSuggestionSelection();
            }
        } else if (e.key === 'ArrowUp') {
            e.preventDefault();
            if (suggestionsContainer.style.display !== 'none' && suggestions.length > 0) {
                selectedSuggestionIndex = Math.max(selectedSuggestionIndex - 1, -1);
                updateSuggestionSelection();
            }
        } else if (e.key === 'Escape') {
            hideSuggestions();
            searchInput.blur();
        }
    });
    
    // Search button click
    searchBtn.addEventListener('click', performSearch);
    
    // Clear button
    clearBtn.addEventListener('click', () => {
        searchInput.value = '';
        clearBtn.style.display = 'none';
        hideSuggestions();
        searchInput.focus();
    });
    
    // Hide suggestions when clicking outside
    document.addEventListener('click', (e) => {
        const suggestionsContainer = document.getElementById('suggestions-container');
        const searchBoxWrapper = document.querySelector('.search-box-wrapper');
        if (!suggestionsContainer.contains(e.target) && !searchBoxWrapper.contains(e.target)) {
            hideSuggestions();
        }
    });
    
    // Back to search button
    const backToSearchBtn = document.getElementById('back-to-search');
    backToSearchBtn.addEventListener('click', () => {
        const resultsContainer = document.getElementById('results-container');
        const main = document.querySelector('main');
        resultsContainer.style.display = 'none';
        main.style.display = 'flex';
        searchInput.focus();
    });
    
    // About button
    const aboutLink = document.getElementById('about-link');
    const aboutModal = document.getElementById('about-modal');
    const aboutModalClose = document.getElementById('about-modal-close');
    
    aboutLink.addEventListener('click', (e) => {
        e.preventDefault();
        showAboutModal();
    });
    
    aboutModalClose.addEventListener('click', () => {
        hideAboutModal();
    });
    
    aboutModal.addEventListener('click', (e) => {
        if (e.target === aboutModal || e.target.classList.contains('about-modal-overlay')) {
            hideAboutModal();
        }
    });
    
    // Close modal on Escape key
    document.addEventListener('keydown', (e) => {
        if (e.key === 'Escape') {
            if (aboutModal.style.display !== 'none') {
                hideAboutModal();
            }
            if (helpModal.style.display !== 'none') {
                hideHelpModal();
            }
            if (termsModal.style.display !== 'none') {
                hideTermsModal();
            }
        }
        
        // Focus search box with Ctrl/Cmd + K
        if ((e.ctrlKey || e.metaKey) && e.key === 'k') {
            e.preventDefault();
            searchInput.focus();
        }
    });
    
    // Terms button
    const termsLink = document.getElementById('terms-link');
    const termsModal = document.getElementById('terms-modal');
    const termsModalClose = document.getElementById('terms-modal-close');
    
    termsLink.addEventListener('click', (e) => {
        e.preventDefault();
        showTermsModal();
    });
    
    termsModalClose.addEventListener('click', () => {
        hideTermsModal();
    });
    
    termsModal.addEventListener('click', (e) => {
        if (e.target === termsModal || e.target.classList.contains('terms-modal-overlay')) {
            hideTermsModal();
        }
    });
    
    // Set current date for terms
    const termsDateElement = document.getElementById('terms-date');
    if (termsDateElement) {
        const currentDate = new Date();
        const options = { year: 'numeric', month: 'long', day: 'numeric' };
        termsDateElement.textContent = currentDate.toLocaleDateString('en-US', options);
    }
    
    // Help button
    const helpLink = document.getElementById('help-link');
    const helpModal = document.getElementById('help-modal');
    const helpModalClose = document.getElementById('help-modal-close');
    
    helpLink.addEventListener('click', (e) => {
        e.preventDefault();
        showHelpModal();
    });
    
    helpModalClose.addEventListener('click', () => {
        hideHelpModal();
    });
    
    helpModal.addEventListener('click', (e) => {
        if (e.target === helpModal || e.target.classList.contains('help-modal-overlay')) {
            hideHelpModal();
        }
    });
    
    // FAQ Accordion functionality
    const faqItems = document.querySelectorAll('.faq-item');
    faqItems.forEach(item => {
        const question = item.querySelector('.faq-question');
        question.addEventListener('click', () => {
            const isActive = item.classList.contains('active');
            
            // Close all other FAQ items
            faqItems.forEach(otherItem => {
                if (otherItem !== item) {
                    otherItem.classList.remove('active');
                }
            });
            
            // Toggle current item
            if (isActive) {
                item.classList.remove('active');
            } else {
                item.classList.add('active');
            }
        });
    });
});

async function checkStatus() {
    try {
        const response = await fetch(`${API_BASE}/api/status`);
        const data = await response.json();
        const statusEl = document.getElementById('status');
        if (data.documentCount > 0) {
            statusEl.textContent = `Indexed ${data.documentCount} document${data.documentCount !== 1 ? 's' : ''}`;
        } else {
            statusEl.textContent = 'No documents indexed. Please add documents to the data folder.';
        }
    } catch (error) {
        console.error('Error checking status:', error);
    }
}

async function fetchSuggestions(query) {
    if (!query || query.length < 1) {
        hideSuggestions();
        return;
    }
    
    try {
        const encodedQuery = encodeURIComponent(query);
        const response = await fetch(`${API_BASE}/api/suggestions?q=${encodedQuery}`);
        const data = await response.json();
        
        suggestions = data.suggestions || [];
        selectedSuggestionIndex = -1;
        
        if (suggestions.length > 0) {
            displaySuggestions(suggestions, query);
        } else {
            hideSuggestions();
        }
    } catch (error) {
        console.error('Error fetching suggestions:', error);
        hideSuggestions();
    }
}

function displaySuggestions(suggestionsList, query) {
    const suggestionsContainer = document.getElementById('suggestions-container');
    const suggestionsListEl = document.getElementById('suggestions-list');
    
    suggestionsListEl.innerHTML = '';
    
    suggestionsList.forEach((suggestion, index) => {
        const suggestionItem = document.createElement('div');
        suggestionItem.className = 'suggestion-item';
        suggestionItem.dataset.index = index;
        
        // Highlight matching part
        const lowerSuggestion = suggestion.toLowerCase();
        const lowerQuery = query.toLowerCase();
        const matchIndex = lowerSuggestion.indexOf(lowerQuery);
        
        if (matchIndex >= 0) {
            const beforeMatch = suggestion.substring(0, matchIndex);
            const match = suggestion.substring(matchIndex, matchIndex + query.length);
            const afterMatch = suggestion.substring(matchIndex + query.length);
            
            suggestionItem.innerHTML = `
                <i class="fas fa-search suggestion-icon"></i>
                <span class="suggestion-text">
                    ${escapeHtml(beforeMatch)}<strong>${escapeHtml(match)}</strong>${escapeHtml(afterMatch)}
                </span>
            `;
        } else {
            suggestionItem.innerHTML = `
                <i class="fas fa-search suggestion-icon"></i>
                <span class="suggestion-text">${escapeHtml(suggestion)}</span>
            `;
        }
        
        suggestionItem.addEventListener('click', () => {
            const searchInput = document.getElementById('search-input');
            searchInput.value = suggestion;
            hideSuggestions();
            performSearch();
        });
        
        suggestionItem.addEventListener('mouseenter', () => {
            selectedSuggestionIndex = index;
            updateSuggestionSelection();
        });
        
        suggestionsListEl.appendChild(suggestionItem);
    });
    
    suggestionsContainer.style.display = 'block';
}

function updateSuggestionSelection() {
    const suggestionItems = document.querySelectorAll('.suggestion-item');
    suggestionItems.forEach((item, index) => {
        if (index === selectedSuggestionIndex) {
            item.classList.add('selected');
        } else {
            item.classList.remove('selected');
        }
    });
}

function hideSuggestions() {
    const suggestionsContainer = document.getElementById('suggestions-container');
    suggestionsContainer.style.display = 'none';
    selectedSuggestionIndex = -1;
    suggestions = [];
}

function escapeHtml(text) {
    const div = document.createElement('div');
    div.textContent = text;
    return div.innerHTML;
}

async function performSearch() {
    const searchInput = document.getElementById('search-input');
    const query = searchInput.value.trim();
    
    if (!query) {
        return;
    }
    
    currentQuery = query;
    const statusEl = document.getElementById('status');
    statusEl.innerHTML = 'Searching<span class="loading"></span>';
    
    try {
        const encodedQuery = encodeURIComponent(query);
        const response = await fetch(`${API_BASE}/api/search?q=${encodedQuery}`);
        const data = await response.json();
        
        displayResults(data.results || [], query);
    } catch (error) {
        console.error('Search error:', error);
        statusEl.textContent = 'Error performing search. Please try again.';
    }
}

function displayResults(results, query) {
    const resultsContainer = document.getElementById('results-container');
    const resultsList = document.getElementById('results-list');
    const resultsCount = document.getElementById('results-count');
    const statusEl = document.getElementById('status');
    const main = document.querySelector('main');
    
    // Hide main search interface
    main.style.display = 'none';
    
    // Show results
    resultsContainer.style.display = 'block';
    
    // Update count
    if (results.length > 0) {
        resultsCount.textContent = `About ${results.length} result${results.length !== 1 ? 's' : ''}`;
        statusEl.textContent = '';
    } else {
        resultsCount.textContent = 'No results found';
        statusEl.textContent = `No results found for "${query}"`;
    }
    
    // Clear previous results
    resultsList.innerHTML = '';
    
    // Display results
    results.forEach((result, index) => {
        const resultItem = createResultItem(result, query);
        resultsList.appendChild(resultItem);
    });
    
    // Scroll to top
    window.scrollTo(0, 0);
}

function createResultItem(result, query) {
    const item = document.createElement('div');
    item.className = 'result-item';
    
    const title = document.createElement('a');
    title.className = 'result-title';
    title.textContent = result.fileName;
    title.href = '#';
    title.addEventListener('click', (e) => {
        e.preventDefault();
        showDocument(result.filePath);
    });
    
    const url = document.createElement('div');
    url.className = 'result-url';
    url.textContent = result.filePath;
    
    const snippet = document.createElement('div');
    snippet.className = 'result-snippet';
    snippet.textContent = `Relevance score: ${result.score.toFixed(4)}`;
    
    const score = document.createElement('div');
    score.className = 'result-score';
    score.textContent = `TF-IDF Score: ${result.score.toFixed(6)}`;
    
    item.appendChild(title);
    item.appendChild(url);
    item.appendChild(snippet);
    item.appendChild(score);
    
    return item;
}

async function showDocument(filePath) {
    try {
        const encodedPath = encodeURIComponent(filePath);
        const response = await fetch(`${API_BASE}/api/document?path=${encodedPath}`);
        const data = await response.json();
        
        // Create modal or new view to show document
        showDocumentModal(data.content, filePath);
    } catch (error) {
        console.error('Error loading document:', error);
        alert('Error loading document content');
    }
}

function showDocumentModal(content, filePath) {
    // Create modal overlay
    const overlay = document.createElement('div');
    overlay.className = 'modal-overlay';
    overlay.style.cssText = `
        position: fixed;
        top: 0;
        left: 0;
        width: 100%;
        height: 100%;
        background: rgba(0, 0, 0, 0.9);
        backdrop-filter: blur(10px);
        z-index: 1000;
        display: flex;
        align-items: center;
        justify-content: center;
        padding: 20px;
        animation: fadeIn 0.3s ease-out;
    `;
    
    // Create modal content
    const modal = document.createElement('div');
    modal.style.cssText = `
        background: rgba(0, 0, 0, 0.95);
        max-width: 900px;
        max-height: 90vh;
        width: 100%;
        display: flex;
        flex-direction: column;
        box-shadow: 0 0 50px rgba(220, 20, 60, 0.3), inset 0 0 50px rgba(220, 20, 60, 0.03);
        border: 1px solid rgba(220, 20, 60, 0.4);
        overflow: hidden;
        animation: slideUp 0.3s ease-out;
        border-radius: 2px;
    `;
    
    // Add animation
    const style = document.createElement('style');
    style.textContent = `
        @keyframes slideUp {
            from {
                opacity: 0;
                transform: translateY(30px);
            }
            to {
                opacity: 1;
                transform: translateY(0);
            }
        }
    `;
    document.head.appendChild(style);
    
    const header = document.createElement('div');
    header.style.cssText = `
        padding: 24px 28px;
        border-bottom: 1px solid rgba(220, 20, 60, 0.3);
        display: flex;
        justify-content: space-between;
        align-items: center;
        background: rgba(0, 0, 0, 0.8);
        box-shadow: 0 2px 20px rgba(220, 20, 60, 0.1);
    `;
    
    const title = document.createElement('h2');
    title.textContent = filePath.split(/[/\\]/).pop();
    title.style.margin = '0';
    title.style.fontSize = '18px';
    title.style.fontWeight = '600';
    title.style.color = 'rgba(220, 20, 60, 0.95)';
    title.style.textTransform = 'uppercase';
    title.style.letterSpacing = '2px';
    title.style.fontFamily = "'Rajdhani', sans-serif";
    
    const closeBtn = document.createElement('button');
    closeBtn.innerHTML = '<i class="fas fa-times"></i>';
    closeBtn.style.cssText = `
        background: rgba(220, 20, 60, 0.1);
        border: 1px solid rgba(220, 20, 60, 0.3);
        font-size: 18px;
        cursor: pointer;
        color: rgba(220, 20, 60, 0.9);
        line-height: 1;
        padding: 8px 12px;
        width: auto;
        height: auto;
        transition: all 0.2s ease;
        display: flex;
        align-items: center;
        justify-content: center;
        border-radius: 2px;
        box-shadow: 0 0 15px rgba(220, 20, 60, 0.15);
    `;
    closeBtn.addEventListener('mouseenter', () => {
        closeBtn.style.background = 'rgba(220, 20, 60, 0.2)';
        closeBtn.style.transform = 'scale(1.1)';
        closeBtn.style.boxShadow = '0 0 25px rgba(220, 20, 60, 0.4)';
        closeBtn.style.borderColor = 'rgba(220, 20, 60, 0.6)';
    });
    closeBtn.addEventListener('mouseleave', () => {
        closeBtn.style.background = 'rgba(220, 20, 60, 0.1)';
        closeBtn.style.transform = 'scale(1)';
        closeBtn.style.boxShadow = '0 0 15px rgba(220, 20, 60, 0.15)';
        closeBtn.style.borderColor = 'rgba(220, 20, 60, 0.3)';
    });
    closeBtn.addEventListener('click', () => {
        overlay.style.animation = 'fadeOut 0.2s ease-out';
        setTimeout(() => {
            if (document.body.contains(overlay)) {
                document.body.removeChild(overlay);
            }
        }, 200);
    });
    
    header.appendChild(title);
    header.appendChild(closeBtn);
    
    const body = document.createElement('div');
    body.style.cssText = `
        padding: 28px;
        overflow-y: auto;
        flex: 1;
        font-family: 'Courier New', 'Monaco', 'Inconsolata', monospace;
        font-size: 14px;
        line-height: 1.8;
        white-space: pre-wrap;
        word-wrap: break-word;
        color: rgba(224, 224, 224, 0.9);
        background: rgba(0, 0, 0, 0.9);
    `;
    body.textContent = content;
    
    modal.appendChild(header);
    modal.appendChild(body);
    overlay.appendChild(modal);
    
    // Close on overlay click
    overlay.addEventListener('click', (e) => {
        if (e.target === overlay) {
            overlay.style.animation = 'fadeOut 0.2s ease-out';
            setTimeout(() => {
                if (document.body.contains(overlay)) {
                    document.body.removeChild(overlay);
                }
            }, 200);
        }
    });
    
    // Close on Escape key
    const handleEscape = (e) => {
        if (e.key === 'Escape') {
            overlay.style.animation = 'fadeOut 0.2s ease-out';
            setTimeout(() => {
                if (document.body.contains(overlay)) {
                    document.body.removeChild(overlay);
                }
            }, 200);
            document.removeEventListener('keydown', handleEscape);
        }
    };
    document.addEventListener('keydown', handleEscape);
    
    document.body.appendChild(overlay);
}

// Handle back button to return to search
window.addEventListener('popstate', () => {
    const resultsContainer = document.getElementById('results-container');
    const main = document.querySelector('main');
    
    if (resultsContainer.style.display === 'block') {
        resultsContainer.style.display = 'none';
        main.style.display = 'flex';
    }
});

function showAboutModal() {
    const aboutModal = document.getElementById('about-modal');
    aboutModal.style.display = 'flex';
    document.body.style.overflow = 'hidden';
    
    // Animate modal appearance
    setTimeout(() => {
        aboutModal.classList.add('active');
    }, 10);
}

function hideAboutModal() {
    const aboutModal = document.getElementById('about-modal');
    aboutModal.classList.remove('active');
    
    setTimeout(() => {
        aboutModal.style.display = 'none';
        document.body.style.overflow = '';
    }, 300);
}

function showHelpModal() {
    const helpModal = document.getElementById('help-modal');
    helpModal.style.display = 'flex';
    document.body.style.overflow = 'hidden';
    
    // Animate modal appearance
    setTimeout(() => {
        helpModal.classList.add('active');
    }, 10);
}

function hideHelpModal() {
    const helpModal = document.getElementById('help-modal');
    helpModal.classList.remove('active');
    
    setTimeout(() => {
        helpModal.style.display = 'none';
        document.body.style.overflow = '';
    }, 300);
}

function showTermsModal() {
    const termsModal = document.getElementById('terms-modal');
    termsModal.style.display = 'flex';
    document.body.style.overflow = 'hidden';
    
    // Animate modal appearance
    setTimeout(() => {
        termsModal.classList.add('active');
    }, 10);
}

function hideTermsModal() {
    const termsModal = document.getElementById('terms-modal');
    termsModal.classList.remove('active');
    
    setTimeout(() => {
        termsModal.style.display = 'none';
        document.body.style.overflow = '';
    }, 300);
}

