import logging
from aiohttp import web
from App.Views import routes
from App.Logger import AccessLogger
import os


args = {
    'ip': os.getenv('IP', '0.0.0.0'),
    'port': os.getenv('PORT', '80')
}

app = web.Application()
app.router.add_routes(routes)
logging.basicConfig(level=logging.INFO)
web.run_app(app, port=args['port'], host=args['ip'], access_log_class=AccessLogger, reuse_address=True)
