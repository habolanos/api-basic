# Etapa de construcción
FROM alpine:3.18 AS build

# Instalar dependencias de construcción
RUN apk update && apk add --no-cache \
    build-base \
    cmake \
    git \
    openssl-dev \
    boost-dev \
    zlib-dev \
    brotli-dev

# Clonar y construir cpprestsdk
RUN git clone --branch v2.10.19 https://github.com/microsoft/cpprestsdk.git /cpprestsdk && \
    mkdir /cpprestsdk/build && cd /cpprestsdk/build && \
    cmake -DCPPREST_EXCLUDE_WEBSOCKETS=ON -DCMAKE_BUILD_TYPE=Release .. && \
    make -j$(nproc) && make install

# Clonar y construir spdlog
RUN git clone https://github.com/gabime/spdlog.git /spdlog && \
    mkdir /spdlog/build && cd /spdlog/build && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    make -j$(nproc) && make install

# Copiar archivos de la aplicación
WORKDIR /app
COPY main.cpp .
COPY swagger.json .

# Construir la aplicación
RUN g++ -std=c++17 -o api-basic main.cpp -lcpprest -lssl -lcrypto -lboost_system -lz -lbrotlienc -lbrotlidec -lspdlog -lpthread

# Etapa final
FROM alpine:3.18

# Instalar dependencias de runtime
RUN apk add --no-cache \
    libstdc++ \
    boost-system \
    openssl \
    zlib \
    brotli-libs

# Crear directorio para logs
RUN mkdir -p /var/log

# Copiar el binario y el archivo swagger
COPY --from=build /app/api-basic /app/api-basic
COPY --from=build /app/swagger.json /app/swagger.json

# Configurar permisos y usuario
RUN addgroup -S appgroup && adduser -S appuser -G appgroup
RUN chown -R appuser:appgroup /var/log /app
USER appuser

# Exponer puerto
EXPOSE 8080

# Comando para ejecutar
CMD ["/app/api-basic"]