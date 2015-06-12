from pylab import *
import sys, os, re, operator, numpy, itertools

def generate_graph(path, table):

    if not os.path.isfile(path):
        print "File not found!"
        sys.exit(0)

    #list of time-size tuples
    sizes = []
    times = []
    with open(path) as file:
        for line in file:
            args = line.strip().split()
            if len(args) == 2:
                sizes.append(int(args[0]))
                times.append(float(args[1]))
            else:
                print "Bad line!"
    if(table == 0):
        plot(sizes, times, color="green", label="Hashtable")

paths = sys.argv[1]
plottitle = sys.argv[2]
generate_graph(paths + "_true", 0)
title(plottitle)
grid(True)
minorticks_on()

legend(loc="lower right")
xlabel("2^x elements")
ylabel("micros per element")
print "generating plot"
savefig(paths + ".pdf")