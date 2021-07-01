import pandas as pd
from multiprocessing import Process
import numpy as np

#DataFrame TUTORES
urlTutor=open('tutor.txt','r')
url0=urlTutor
tutores=pd.read_csv(url0)
df0=pd.DataFrame(tutores)
print(url0)