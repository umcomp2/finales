from celery import Celery
import smtplib, ssl

app= Celery('tasks',broker='amqp://localhost//') #podría ir una ip si fuera remoto es el broker ce rabbit este

@app.task
def send_email(sender,password,send_to,msg):
    port = 465

    message = """\
    Subject: PYlogger Defense system

    WARNING! One of your sensors passed the limit\n 
    """+str(msg)+"""

    Contact information: +542616123311 -Roi
    """

    context = ssl.create_default_context()
    with smtplib.SMTP_SSL("smtp.gmail.com", port, context=context) as server:
        server.login(sender, password)
        server.sendmail(sender, send_to, message)

    return ("alert sent")
