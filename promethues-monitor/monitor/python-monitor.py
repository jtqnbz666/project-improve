# 业务自定义监控
import web
from prometheus_client import Gauge, generate_latest, CONTENT_TYPE_LATEST
import web
from VRedisScript import register_script
# 运营消息队列
#grsc = redis.StrictRedis(host = '127.0.0.1', port = 6379, password = "", socket_timeout = 10, socket_connect_timeout = 10, lib_name = "", lib_version="")
from VSJGlobalVariable import grsc
PROCESS_MEMORY = Gauge('team_num_monitor', 'team_num_report', ['svrdname', 'port'])
urls = (
    '/metrics', 'Metrics'
)

class Metrics:
    def GET(self):
        # 逻辑：读取“上一分钟”的 Key
        # time.time() 获取当前时间戳，减去 60 秒
        key = "test"
        # 从 Redis 获取数据
        data = grsc.hgetall(key)
        print('data--', data, flush=True)
        # 必须先清空，防止旧进程残留
        PROCESS_MEMORY.clear()
        for key_data, value in data.items():
            print('key_data--', key_data, flush=True)
            print('value--', value, flush=True)
            values = key_data.split('_')
            svrdname, port = values[0], values[1]
            PROCESS_MEMORY.labels(svrdname=svrdname, port=port).set(float(value))

		# 设置响应头
        web.header('Content-Type', CONTENT_TYPE_LATEST)
        # 返回 Prometheus 格式的字符串
        return generate_latest()

if __name__ == "__main__":
    app = web.application(urls, globals(), autoreload=False) # 关键：禁用自动重载
    app.run()
