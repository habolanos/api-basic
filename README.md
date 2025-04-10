# API Basic (Python)

A lightweight REST API built with FastAPI for addition and subtraction operations, with health check and integrated Swagger UI.

### **Manifiesto de Kubernetes**
El manifiesto ya está configurado para exponer la API en el puerto 8080, donde Swagger UI está disponible. No requiere ajustes:

```yaml
apiVersion: apps/v1
kind: Deployment
metadata:
  name: api-basic
  labels:
    app: api-basic
spec:
  replicas: 1
  selector:
    matchLabels:
      app: api-basic
  template:
    metadata:
      labels:
        app: api-basic
    spec:
      containers:
      - name: api-basic
        image: api-basic:latest
        ports:
        - containerPort: 8080
        volumeMounts:
        - name: log-volume
          mountPath: /var/log
        resources:
          limits:
            cpu: "0.5"
            memory: "128Mi"
          requests:
            cpu: "0.1"
            memory: "64Mi"
        livenessProbe:
          httpGet:
            path: /health
            port: 8080
          initialDelaySeconds: 5
          periodSeconds: 30
          timeoutSeconds: 3
          failureThreshold: 3
        readinessProbe:
          httpGet:
            path: /health
            port: 8080
          initialDelaySeconds: 5
          periodSeconds: 10
          timeoutSeconds: 3
          failureThreshold: 3
      volumes:
      - name: log-volume
        emptyDir: {}
---
apiVersion: v1
kind: Service
metadata:
  name: api-basic-service
spec:
  selector:
    app: api-basic
  ports:
    - protocol: TCP
      port: 8080
      targetPort: 8080
  type: LoadBalancer
```

---

## Features
- **Endpoints**:
  - `GET /add/{num1}/{num2}`: Adds two numbers.
  - `GET /subtract/{num1}/{num2}`: Subtracts two numbers.
  - `GET /health`: Returns health status.
- **Swagger UI**: Interactive API documentation at `http://localhost:8080/docs`.
- Logging to `/var/log/api-basic.log`.
- Containerized with Docker (Python Slim base).
- Healthchecks for Docker and Kubernetes.

## Prerequisites
- Docker
- Kubernetes (optional)

## Build ம

Build and Run with Docker

1. **Build the Docker image**:
   ```bash
   docker build -t api-basic .
   ```

2. **Run the container**:
   ```bash
   docker run -d -p 8080:8080 -v $(pwd)/logs:/var/log api-basic
   ```

3. **Check health status**:
   ```bash
   docker ps  # Verifica que el estado sea "healthy"
   ```

4. **Access the API**:
   - Swagger UI: `http://localhost:8080/docs`
   - Addition: `http://localhost:8080/add/5/3`
   - Subtraction: `http://localhost:8080/subtract/5/3`
   - Health: `http://localhost:8080/health`

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

4. **Check pod status**:
   ```bash
   kubectl get pods  # Verifica que el pod esté "Running" y "Ready"
   ```

5. **Access the service**:
   ```bash
   kubectl get svc api-basic-service
   ```
   Use the external IP to access the API.

## Swagger UI
- Access the interactive Swagger UI at `http://localhost:8080/docs` (or the external IP in Kubernetes).
- Test endpoints directly from the UI by clicking "Try it out" and entering values.

## Logging
Logs are written to `/var/log/api-basic.log` inside the container. Mount a volume to persist logs locally (e.g., `./logs:/var/log`).

## Healthchecks
- **Docker**: Uses `HEALTHCHECK` to verify `/health` every 30 seconds.
- **Kubernetes**: Includes `livenessProbe` (restarts if unhealthy) and `readinessProbe` (controls traffic) on `/health`.

## Dependencies
- `fastapi`: Lightweight REST framework with OpenAPI support.
- `uvicorn`: ASGI server to run the API.
- Python 3.11 Slim: Lightweight base image (~80-100 MB).

## Notes
- The API runs on port `8080`.
- Swagger UI is served at `/docs`, with raw OpenAPI JSON at `/openapi.json`.


---

### **Personalización Opcional del Swagger UI**
Si quieres personalizar el Swagger UI (e.g., cambiar el título, agregar un logo, o deshabilitar ciertas funciones), puedes modificar el objeto `FastAPI` en `main.py`. Por ejemplo:

```python
app = FastAPI(
    title="API Basic",
    description="A simple API for addition and subtraction",
    version="1.0.0",
    docs_url="/docs",  # Ruta por defecto para Swagger UI
    redoc_url="/redoc",  # Alternativa a Swagger UI
    openapi_url="/openapi.json"  # Ruta para el JSON de OpenAPI
)
```

- Para deshabilitar Swagger UI y usar solo ReDoc:
  ```python
  app = FastAPI(docs_url=None, redoc_url="/redoc")
  ```

---

### **Probar el Swagger UI**
1. Construye y ejecuta el contenedor:
   ```bash
   docker build -t api-basic .
   docker run -d -p 8080:8080 -v $(pwd)/logs:/var/log api-basic
   ```
2. Abre tu navegador en `http://localhost:8080/docs`.
3. Verás una interfaz interactiva con los endpoints `/add`, `/subtract`, y `/health`. Haz clic en "Try it out" para probarlos directamente.