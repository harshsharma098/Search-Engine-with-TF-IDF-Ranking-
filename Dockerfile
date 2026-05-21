FROM ubuntu:24.04 AS build

RUN apt-get update \
    && apt-get install -y --no-install-recommends build-essential cmake \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app/SearchEngine
COPY SearchEngine/ ./

RUN rm -rf build \
    && cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \
    && cmake --build build --config Release

FROM ubuntu:24.04

WORKDIR /app/SearchEngine
COPY --from=build /app/SearchEngine/build/bin/SearchEngine ./SearchEngine
COPY SearchEngine/web ./web
COPY SearchEngine/data ./data

CMD ["./SearchEngine"]
