#alert process send a mail when a value goes higher than spected
import smtplib, ssl

def sendmail(qa,configuration):
    port = 465

    sender = configuration[6] 
    password = configuration[7]
    recieve = configuration[8]

    while True:
        if not qa.empty():
            message = """\
            Subject: PYlogger Defense system

            WARNING! One of your sensors passed the limit\n 
            """+str(qa.get())+"""

            Contact information: +542616123311 -Roi
            """

            context = ssl.create_default_context()
            print('\n\n---------------------------------')
            print("\tSENDING ALERT")
            with smtplib.SMTP_SSL("smtp.gmail.com", port, context=context) as server:
                server.login(sender, password)
                server.sendmail(sender, recieve, message)

            print("\tALERT SENT!\n------------------------------")
