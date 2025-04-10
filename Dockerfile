# Etapa de construcción
FROM python:3.11-slim AS build

# Instalar dependencias necesarias para construir
RUN apt-get update && apt-get install -y --no-install-recommends \
    gcc \
    && rm -rf /var/lib/apt/lists/*

# Establecer directorio de trabajo
WORKDIR /app

# Copiar archivo de la aplicación
COPY main.py .

# Instalar dependencias en un entorno virtual
RUN python -m venv /venv && \
    /venv/bin/pip install --no-cache-dir fastapi uvicorn

# Etapa final
FROM python:3.11-slim

# Instalar solo las dependencias de runtime y curl para el healthcheck
RUN apt-get update && apt-get install -y --no-install-recommends \
    curl \
    && rm -rf /var/lib/apt/lists/*

# Copiar el entorno virtual desde la etapa de construcción
COPY --from=build /venv /venv
COPY --from=build /app/main.py /app/main.py

# Crear directorio para logs y ajustar permisos
RUN mkdir -p /var/log && chmod -R 777 /var/log

# Establecer directorio de trabajo
WORKDIR /app

# Usar el entorno virtual
ENV PATH="/venv/bin:$PATH"

# Exponer puerto
EXPOSE 8080

# Healthcheck usando el endpoint /health
HEALTHCHECK --interval=30s --timeout=3s --start-period=5s --retries=3 \
    CMD curl -f http://localhost:8080/health || exit 1

# Comando para ejecutar la API
CMD ["uvicorn", "main:app", "--host", "0.0.0.0", "--port", "8080"]