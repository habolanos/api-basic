# API-Basic

A simple C++ REST API built with `cpprestsdk` for performing addition and subtraction operations, with health check and Swagger documentation.

## Features
- **Endpoints**:
  - `GET /add/{num1}/{num2}`: Adds two numbers.
  - `GET /subtract/{num1}/{num2}`: Subtracts two numbers.
  - `GET /health`: Returns health status.
  - `GET /`: Serves Swagger JSON.
- Logging with `spdlog` to `/var/log/api-basic.log`.
- Containerized with Docker (Alpine base).

## Prerequisites
- Docker
- Docker Compose (optional)
- Kubernetes (optional)

## Build and Run with Docker

1. **Build the Docker image**:
   ```bash
   docker build -t api-basic .
   ```

2. **Run the container**:
   ```bash
   docker run -d -p 8080:8080 -v $(pwd)/logs:/var/log api-basic
   ```

3. **Access the API**:
   - Swagger: `http://localhost:8080/`
   - Addition: `http://localhost:8080/add/5/3`
   - Subtraction: `http://localhost:8080/subtract/5/3`
   - Health: `http://localhost:8080/health`

## Run with Docker Compose

1. **Start the service**:
   ```bash
   docker-compose up --build
   ```

2. **Stop the service**:
   ```bash
   docker-compose down
   ```

## Deploy to Kubernetes

1. **Build and tag the image**:
   ```bash
   docker build -t api-basic:latest .
   ```

2. **Push to a registry (optional)**:
   Replace `your-registry` with your container registry:
   ```bash
   docker tag api-basic:latest your-registry/api-basic:latest
   docker push your-registry/api-basic:latest
   ```

3. **Apply the manifest**:
   Update the `image` field in `k8s-deployment.yml` if using a custom registry.
   ```bash
   kubectl apply -f k8s-deployment.yml
   ```

4. **Access the service**:
   ```bash
   kubectl get svc api-basic-service
   ```
   Use the external IP to access the API.

## Logging
Logs are written to `/var/log/api-basic.log` inside the container. Mount a volume to persist logs locally (e.g., `./logs:/var/log`).

## Dependencies
- `cpprestsdk`: REST API framework.
- `spdlog`: Logging library.
- Alpine Linux: Lightweight base image.

## Notes
- The API runs on port `8080`.
- Swagger JSON is served at the root (`/`), but you'll need a Swagger UI client to visualize it (e.g., paste into `https://editor.swagger.io/`).

---

### **8. Pasos Finales**

1. **Probar localmente**:
   - Coloca todos los archivos en una carpeta `api-basic`.
   - Ejecuta `docker build -t api-basic .` y luego `docker run -d -p 8080:8080 -v $(pwd)/logs:/var/log api-basic`.
   - Usa `curl` o un navegador para probar los endpoints.

2. **Validar Swagger**:
   - Visita `http://localhost:8080/` para obtener el JSON y pégalo en `https://editor.swagger.io/` para visualizarlo.

3. **Ver logs**:
   - Revisa `./logs/api-basic.log` después de ejecutar el contenedor.