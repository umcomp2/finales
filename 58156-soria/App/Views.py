from aiohttp.web import View, FileResponse, RouteTableDef, Response
from .Maze import Maze
from .Table import Table
import pathlib


this_path = str(pathlib.Path(__file__).parent.absolute())
routes = RouteTableDef()
routes.static('/static', this_path + '/static')


@routes.view('/')
@routes.view('/index.html')
class Index(View):

    async def get(self):
        return FileResponse(path=this_path + '/index.html', status=200)

    async def post(self):
        data = await self.request.post()
        maze_file = data['maze']
        filename = maze_file.filename
        if '.txt' not in filename:
            raise Exception
        txt_file = maze_file.file
        content = txt_file.read().decode("utf-8")
        maze = Maze(content)
        solution = maze.solve()
        web_solution = Table(maze.maze, solution)
        site_generated = web_solution.generate_html()
        return Response(body=site_generated, status=200, content_type='text/html')


@routes.view('/favicon.ico')
class Favicon(View):

    async def get(self):
        return FileResponse(path=this_path + '/static/favicon.ico', status=200)
