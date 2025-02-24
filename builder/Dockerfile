FROM debian:latest

WORKDIR /src
RUN echo "Acquire::http { Proxy \"http://192.168.1.3:3142\"; };\n" > /etc/apt/apt.conf.d/02Proxy
RUN apt-get update && \
    apt-get install -y build-essential cmake libqt5bluetooth5 libbluetooth-dev qt6-connectivity-dev \
    qt6-tools-dev git nano mc less htop libqt6serialport6 qt6-serialport-dev && \
    apt-get clean