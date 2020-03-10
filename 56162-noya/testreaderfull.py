import file_funcs as ff
import multiprocessing

lk_file = multiprocessing.Lock()

ff.reader_full(lk_file)
avg_temperature, avg_humidity = ff.average()

print(avg_temperature,avg_humidity)
