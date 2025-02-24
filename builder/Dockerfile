FROM debian

WORKDIR /src
RUN echo "Acquire::http::Proxy "http://192.168.3.1:3142";" > /etc/apt/apt.conf
RUN apt-get update && \
    apt-get install -y build-essential cmake libqt5bluetooth5 libbluetooth-dev qt6-connectivity-dev \
    qt6-tools-dev git nano mc less htop libqt6serialport6 qt6-serialport-dev && \
    apt-get clean

TAG vterranz/discobuild:latest