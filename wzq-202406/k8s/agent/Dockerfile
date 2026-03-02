# Dockerfile for gm-tool-agent

# Build stage
FROM golang:1.22-alpine AS builder

WORKDIR /app

# Copy go.mod and go.sum files
COPY gm-tool-agent/go.mod gm-tool-agent/go.sum ./
# Copy vendor directory
COPY gm-tool-agent/vendor/ ./vendor/

# Copy the source code
COPY gm-tool-agent/ .

# Build the Go app using vendored dependencies
RUN CGO_ENABLED=0 GOOS=linux go build -mod=vendor -a -installsuffix cgo -o gm-tool-agent main.go

# Final stage
FROM alpine:latest

RUN apk --no-cache add ca-certificates

WORKDIR /root/

# Copy the pre-built binary from the builder stage
COPY --from=builder /app/gm-tool-agent .

# Copy the configuration file
COPY gm-tool-agent/conf/server.ini ./conf/

# Expose port 21328 to the outside world
EXPOSE 21328

# Command to run the executable
CMD ["./gm-tool-agent"] 