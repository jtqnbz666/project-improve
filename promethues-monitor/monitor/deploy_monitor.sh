#!/bin/bash

# 1. 创建目录
mkdir -p ~/monitor
cd ~/monitor

# 2. 写入 docker-compose.yml
cat > docker-compose.yml <<EOF
version: '3.3'

services:
  prometheus:
    image: prom/prometheus:latest
    container_name: prometheus
    restart: always
    volumes:
      - ./prometheus.yml:/etc/prometheus/prometheus.yml
      - prometheus_data:/prometheus
    command:
      - '--config.file=/etc/prometheus/prometheus.yml'
      - '--storage.tsdb.path=/prometheus'
      - '--web.console.libraries=/usr/share/prometheus/console_libraries'
      - '--web.console.templates=/usr/share/prometheus/consoles'
      - '--web.enable-lifecycle'
    ports:
      - "9090:9090"
    networks:
      - monitor-net
    extra_hosts:
      - "host.docker.internal:172.17.0.1"

  grafana:
    image: grafana/grafana:latest
    container_name: grafana
    restart: always
    volumes:
      - grafana_data:/var/lib/grafana
    environment:
      - GF_SECURITY_ADMIN_USER=admin
      - GF_SECURITY_ADMIN_PASSWORD=admin
      - GF_USERS_ALLOW_SIGN_UP=false
    ports:
      - "3000:3000"
    networks:
      - monitor-net

  node-exporter:
    image: prom/node-exporter:latest
    container_name: node-exporter
    restart: always
    volumes:
      - /proc:/host/proc:ro
      - /sys:/host/sys:ro
      - /:/rootfs:ro
    command:
      - '--path.procfs=/host/proc'
      - '--path.sysfs=/host/sys'
      - '--collector.filesystem.mount-points-exclude=^/(sys|proc|dev|host|etc)(\$\$|/)'
    network_mode: "host"

  nginx-exporter:
    image: nginx/nginx-prometheus-exporter:latest
    container_name: nginx-exporter
    restart: always
    command:
      - '-nginx.scrape-uri=http://127.0.0.1:80/stub_status'
    network_mode: "host"

volumes:
  prometheus_data:
  grafana_data:

networks:
  monitor-net:
    driver: bridge
EOF

# 3. 写入 prometheus.yml
cat > prometheus.yml <<EOF
global:
  scrape_interval: 15s
  evaluation_interval: 15s

scrape_configs:
  - job_name: 'prometheus'
    static_configs:
      - targets: ['localhost:9090']

  - job_name: 'node_exporter'
    static_configs:
      - targets: ['host.docker.internal:9100']

  - job_name: 'nginx_exporter'
    static_configs:
      - targets: ['host.docker.internal:9113']
EOF

echo "配置文件已生成到 ~/monitor 目录。"
echo "正在尝试启动..."

# 4. 启动
sudo docker-compose up -d

echo "部署完成！请确保 Nginx 已经配置了 stub_status。"
