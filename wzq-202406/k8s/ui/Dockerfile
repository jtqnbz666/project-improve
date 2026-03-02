# Dockerfile for gm-tool-ui

# Build stage
FROM node:16-alpine AS build
WORKDIR /app
COPY gm-tool-ui/package.json ./
COPY gm-tool-ui/package-lock.json ./
RUN npm install
COPY gm-tool-ui/ ./
RUN npm run build

# Production stage
FROM nginx:stable-alpine
COPY --from=build /app/build /usr/share/nginx/html
# Copy nginx config
COPY k8s/ui/nginx.conf /etc/nginx/conf.d/default.conf
EXPOSE 80
CMD ["nginx", "-g", "daemon off;"] 