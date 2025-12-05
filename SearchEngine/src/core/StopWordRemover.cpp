#include "core/StopWordRemover.h"

StopWordRemover::StopWordRemover() {
    initializeStopWords();
}

void StopWordRemover::initializeStopWords() {
    // Common English stop words
    std::vector<std::string> words = {
        "a", "an", "and", "are", "as", "at", "be", "by", "for", "from",
        "has", "he", "in", "is", "it", "its", "of", "on", "that", "the",
        "to", "was", "were", "will", "with", "the", "this", "but", "they",
        "have", "had", "what", "said", "each", "which", "their", "time",
        "if", "up", "out", "many", "then", "them", "these", "so", "some",
        "her", "would", "make", "like", "into", "him", "has", "two", "more",
        "very", "after", "words", "long", "than", "first", "been", "call",
        "who", "oil", "sit", "now", "find", "down", "day", "did", "get",
        "come", "made", "may", "part", "over", "new", "sound", "take",
        "only", "little", "work", "know", "place", "year", "live", "me",
        "back", "give", "most", "very", "after", "thing", "our", "just",
        "name", "good", "sentence", "man", "think", "say", "great", "where",
        "help", "through", "much", "before", "line", "right", "too", "mean",
        "old", "any", "same", "tell", "boy", "follow", "came", "want",
        "show", "also", "around", "form", "three", "small", "set", "put",
        "end", "does", "another", "well", "large", "must", "big", "even",
        "such", "because", "turn", "here", "why", "ask", "went", "men",
        "read", "need", "land", "different", "home", "us", "move", "try",
        "kind", "hand", "picture", "again", "change", "off", "play", "spell",
        "air", "away", "animal", "house", "point", "page", "letter", "mother",
        "answer", "found", "study", "still", "learn", "should", "america",
        "world", "high", "every", "near", "add", "food", "between", "own",
        "below", "country", "plant", "last", "school", "father", "keep",
        "tree", "never", "start", "city", "earth", "eye", "light", "thought",
        "head", "under", "story", "saw", "left", "don't", "few", "while",
        "along", "might", "close", "something", "seem", "next", "hard",
        "open", "example", "begin", "life", "always", "those", "both",
        "paper", "together", "got", "group", "often", "run", "important",
        "until", "children", "side", "feet", "car", "mile", "night", "walk",
        "white", "sea", "began", "grow", "took", "river", "four", "carry",
        "state", "once", "book", "hear", "stop", "without", "second",
        "later", "miss", "idea", "enough", "eat", "face", "watch", "far",
        "indian", "really", "almost", "let", "above", "girl", "sometimes",
        "mountain", "cut", "young", "talk", "soon", "list", "song", "leave",
        "family", "it's"
    };
    
    for (const auto& word : words) {
        stopWords.insert(word);
    }
}

std::vector<std::string> StopWordRemover::removeStopWords(const std::vector<std::string>& tokens) {
    std::vector<std::string> filtered;
    
    for (const auto& token : tokens) {
        if (!isStopWord(token)) {
            filtered.push_back(token);
        }
    }
    
    return filtered;
}

bool StopWordRemover::isStopWord(const std::string& word) const {
    return stopWords.find(word) != stopWords.end();
}

