import numpy as np
import bokeh
import scipy.special

from bokeh.layouts import gridplot
from bokeh.plotting import figure, show, output_file

data = np.genfromtxt("/theory/tecke/geant4/D2O_Sphere_I-build/Energy_Histogram_h1_Energy.csv", delimiter=",", usecols=0, skip_header=9 )

print data


p = figure(title="Energy Deposited in Detector", tools="save", background_fill_color="#E8DDCB")

hist, edges = np.histogram(data, density=True, bins=100)

p.quad(top=hist, bottom=0, left=edges[:-1], right=edges[1:], fill_color="#036564", line_color="#033649")

output_file('histogram.html', title="histogram.py example")

show(p)
