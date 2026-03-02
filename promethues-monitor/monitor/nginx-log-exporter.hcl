listen {
  port = 4040
  metrics_endpoint = "/metrics"
}

namespace "nginx" {
  format = "$remote_addr - $remote_user [$time_local] \"$request\" $status $body_bytes_sent \"$http_referer\" \"$http_user_agent\" $request_time $upstream_response_time_text"

  source {
    files = [
      "/var/log/nginx/access.log"
    ]
  }

  histogram_buckets = [.005, .01, .025, .05, .1, .25, .5, 1, 2.5, 5, 10]

  relabel "method" {
    from = "request"
    split = 1
  }

  relabel "path" {
    from = "request"
    split = 2
  }
}