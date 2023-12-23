FROM python:3.12.1-alpine3.19
WORKDIR /app

RUN pip install --no-cache-dir paho-mqtt soco

COPY *.py /app/
COPY *.sh /app/

CMD ["./run_servers.sh"]
