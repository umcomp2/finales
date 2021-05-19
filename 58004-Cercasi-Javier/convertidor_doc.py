import os
import subprocess
from pdf2docx import Converter

def pdf_to_word(entrada_pdf, q, adr, extension_out):
    try:
        salida_doc = entrada_pdf.split(".")[0]+'.docx'
        cv = Converter(entrada_pdf)
        cv.convert(salida_doc, start=0, end=None)
        cv.close()
        os.system ("clear") 
        print(adr)
        q.put(salida_doc)
    except:
        q.put("Error")

def word_to_pdf(entrada_word, q, adr, extension_out):
    try:
        salida_pdf = entrada_word.split(".")[0]+'.pdf'
        output = subprocess.check_output(['abiword', '--to=pdf',entrada_word])
        os.system ("clear") 
        print(adr)
        q.put(salida_pdf)
    except:
        q.put("Error")
