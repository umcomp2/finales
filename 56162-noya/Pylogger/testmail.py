import alert as al
import multiprocessing
qa = multiprocessing.Queue() #queue for the alert process
lst = [1,49.00,30.00]
qa.put(lst)
al.sendmail(qa)
