class Table:

    def __init__(self, maze, solution) -> None:
        self.maze = maze
        self.solution = solution
        self.start = """<!DOCTYPE html5>
<html>
    <head>
        <title>PMZ - Solution</title>
        <link rel="stylesheet" href="/static/styles.css">
    </head>
    <body>
      <div class="header">
        <a href="/index.html"><img src="/static/favicon.ico"></a>
        <div class="left">
          <nav>
            <p>PMZ</p>
          </nav>
        </div>
        <div class="right">
          <nav class="active">
            <a href="/index.html">Index</a>
          </nav>
        </div>
      </div>
      <div class="maze-content">
        <table class="maze">
        """
        self.end = """        </table>
      </div>
    </body>
</html>
        """
        self.middle = None

    def generate_html(self) -> str:
        self.middle = ""
        for row in range(len(self.maze)):
            self.middle += "<tr>"
            for cell in range(len(self.maze[row])):
                char = self.maze[row][cell]
                if char == '#':
                    style = 'wall'
                elif char == '*':
                    style = 'start'
                elif char == '+':
                    style = 'end'
                elif (row, cell) in self.solution:
                    style = 'path'
                else:
                    style = 'space'
                self.middle += f'<td class="{style}">{char}</td>'
            self.middle += "</tr>"
        return self.start + self.middle + self.end
