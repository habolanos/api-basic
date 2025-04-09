FROM alpine:3.19

RUN apk add --no-cache \
    g++ \
    cmake \
    make \
    git \
    boost-dev \
    openssl-dev

WORKDIR /app

COPY . /app

# Instalar spdlog
RUN git clone https://github.com/gabime/spdlog.git && \
    cd spdlog && mkdir build && cd build && \
    cmake .. && make -j && make install

# Instalar Crow
RUN git clone https://github.com/CrowCpp/Crow.git && \
    mkdir -p /usr/local/include && \
    cp -r Crow/include/* /usr/local/include/

# Descargar Swagger UI
RUN git clone https://github.com/swagger-api/swagger-ui.git && \
    mkdir -p /app/static && \
    cp swagger-ui/dist/* /app/static/ && \
    sed -i 's|https://petstore.swagger.io/v2/swagger.json|/swagger.yaml|' /app/static/index.html

# Compilar la aplicación
RUN g++ -o api-basic main.cpp -I/usr/local/include -lspdlog -lpthread -O2

EXPOSE 8080

RUN mkdir -p /app/logs

CMD ["./api-basic"]