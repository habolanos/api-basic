```markdown
# API-Basic

API escrita en C++ con Crow para realizar operaciones matemáticas básicas (suma y resta) y un healthcheck. Incluye logging con spdlog, está contenerizada con Docker usando Alpine como base y sirve su propia documentación Swagger.

## Requisitos
- Docker
- Kubernetes (opcional, para despliegue en cluster)
- Git

## Construcción y Ejecución con Docker

1. Clona el repositorio:
   ```bash
   git clone <URL_DEL_REPOSITORIO>
   cd api-basic
   ```

2. Construye la imagen Docker:
   ```bash
   docker build -t api-basic:latest .
   ```

3. Ejecuta el contenedor:
   ```bash
   docker run -d -p 8080:8080 --name api-basic api-basic:latest
   ```

4. Prueba la API:
   - Suma: `curl http://localhost:8080/add/2/3`
   - Resta: `curl http://localhost:8080/subtract/5/2`
   - Healthcheck: `curl http://localhost:8080/healthcheck`

5. Verifica los logs:
   Los logs se generan en el directorio `/app/logs/api.log` dentro del contenedor.

## Despliegue en Kubernetes

1. Crea un manifiesto Kubernetes (`k8s-deployment.yaml`):
   ```yaml
   apiVersion: apps/v1
   kind: Deployment
   metadata:
     name: api-basic-deployment
   spec:
     replicas: 3
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
           - mountPath: /app/logs
             name: log-volume
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
       port: 80
       targetPort: 8080
     type: LoadBalancer
   ```

2. Aplica el manifiesto:
   ```bash
   kubectl apply -f k8s-deployment.yaml
   ```

3. Verifica el despliegue:
   ```bash
   kubectl get pods
   kubectl get svc
   ```

4. Accede a la API usando la IP externa del servicio (`LoadBalancer`).

## Documentación de la API con Swagger

La documentación Swagger está integrada en la API y se sirve directamente desde el endpoint `/swagger`. Sigue estos pasos para acceder:

1. **Ejecuta la API**:
   - Asegúrate de que el contenedor esté corriendo (ver sección "Construcción y Ejecución con Docker").

2. **Accede a Swagger UI**:
   - Abre tu navegador en `http://localhost:8080/swagger`.
   - Esto cargará la interfaz de Swagger UI con la documentación de la API.

3. **Obtén el archivo Swagger crudo (opcional)**:
   - Visita `http://localhost:8080/swagger.yaml` para descargar el archivo YAML directamente.

4. **Prueba un Endpoint**:
   - En la interfaz de Swagger UI, selecciona `/add/{num1}/{num2}`.
   - Ingresa `2` y `3` como parámetros y haz clic en "Try it out".
   - Respuesta esperada: `Resultado de la suma: 5`.

### Notas
- El archivo `swagger.yaml` y los archivos estáticos de Swagger UI están empaquetados en el contenedor Docker.
- Asegúrate de que el puerto 8080 esté accesible.

## Endpoints
- `GET /add/{num1}/{num2}`: Suma dos números.
- `GET /subtract/{num1}/{num2}`: Resta dos números.
- `GET /healthcheck`: Verifica el estado de la API.
- `GET /swagger`: Interfaz Swagger UI.
- `GET /swagger.yaml`: Archivo Swagger en formato YAML.

## Logging
Los logs se generan con spdlog y se almacenan en `/app/logs/api.log` dentro del contenedor.

#### Organización de Archivos
```
api-basic/
├── main.cpp           # Código fuente de la API
├── Dockerfile         # Configuración para Docker
├── swagger.yaml       # Documentación Swagger
├── static/            # Archivos de Swagger UI (descargados en el Dockerfile)
│   └── index.html     # Página principal de Swagger UI (modificada)
├── logs/              # Directorio para logs (creado en runtime)
└── README.md          # Instrucciones del proyecto
```

---

### Notas
- **Swagger UI**: El Dockerfile descarga Swagger UI y modifica `index.html` para que apunte a `/swagger.yaml` por defecto.
- **Prueba**: Después de construir y ejecutar el contenedor (`docker build -t api-basic:latest && docker run -d -p 8080:8080 api-basic:latest`), visita `http://localhost:8080/swagger` para ver la interfaz.

---
By habolanos