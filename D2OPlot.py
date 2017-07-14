import numpy as np
import bokeh
import scipy.special

from bokeh.layouts import gridplot
from bokeh.plotting import figure, show, output_file
from bokeh.models import HoverTool

np.set_printoptions(threshold='nan')

# Read in data from the histogram output by GEANT
data = np.genfromtxt("/theory/tecke/geant4/D2O_Sphere_I-build/Energy_Histogram_h1_Energy.csv", delimiter=",", usecols=0, skip_header=8 )

f = open("hist_output","w")

#print data

p = figure(title="Energy Deposited in Detector", background_fill_color="#4473bf", tools="pan,wheel_zoom,box_zoom,reset,hover", toolbar_location="right")
hover = p.select(dict(type=HoverTool))
hover.tooltips = [("Energy", "$index")]
#hover.mode="vline"

edges = np.linspace(0.1,15.1,1501)

p.quad(top=data, bottom=0, left=edges[:-1], right=edges[1:], fill_color="#293f7c", line_color="#FFFFFF")

p.xaxis.axis_label="Energy Deposited by Neutrons"
p.yaxis.axis_label="Number of Neutrons"

output_file('histogram.html', title="Energy Deposited in Detector by Neutrons")

#f.write(np.array2string(hist))
#f.close()

show(p)
