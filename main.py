from fastapi import FastAPI, HTTPException
import logging

# Configurar logging
logging.basicConfig(
    filename="/var/log/api-basic.log",
    level=logging.INFO,
    format="%(asctime)s [%(levelname)s] %(message)s",
)
logger = logging.getLogger("api-basic")

app = FastAPI(
    title="API Basic",
    description="A simple API for addition and subtraction",
    version="1.0.0",
)

@app.get("/add/{num1}/{num2}", summary="Add two numbers")
async def add(num1: int, num2: int):
    try:
        result = num1 + num2
        logger.info(f"Addition: {num1} + {num2} = {result}")
        return {"result": result, "message": "Addition successful"}
    except Exception as e:
        logger.error(f"Error in addition: {str(e)}")
        raise HTTPException(status_code=400, detail="Invalid numbers")

@app.get("/subtract/{num1}/{num2}", summary="Subtract two numbers")
async def subtract(num1: int, num2: int):
    try:
        result = num1 - num2
        logger.info(f"Subtraction: {num1} - {num2} = {result}")
        return {"result": result, "message": "Subtraction successful"}
    except Exception as e:
        logger.error(f"Error in subtraction: {str(e)}")
        raise HTTPException(status_code=400, detail="Invalid numbers")

@app.get("/health", summary="Health check")
async def health():
    logger.info("Health check requested")
    return {"status": "OK"}