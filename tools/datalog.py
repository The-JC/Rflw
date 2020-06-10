import sys
import pandas as pd
import matplotlib
import matplotlib.pyplot as plt

def readFile(file, prescaler=1):
	f = open(file, "r")
	values_set = []
	values_x = []
	ticks = []
	for i, l in enumerate(f):
		if l == '' or l == '\n':
				continue
		l = l.replace('\x01', '')
		try:
			l = l.split(' ')

			if l[1] == 'ffff' or l[2] == 'ffff':
				continue

			tick = int(l[0], 16)/1000
			value_x = int(l[1], 16)/4
			value_set = int(l[2], 16)

			ticks.append(tick)
			values_x.append(value_x)
			values_set.append(value_set)

		except:
			print('Error converting in line ', i+1, ' ', l)

	d = {'time': ticks, 'set': values_set, 'x': values_x}
	return pd.DataFrame(data=d)	

if __name__ == "__main__":
	print(sys.argv)
	if len(sys.argv) != 3 and len(sys.argv) != 4:
		raise ValueError("Usage `python datalog.py [input data] (output file) [Name]")
	
	print("Converting file ", sys.argv[1])
	data = readFile(sys.argv[1])

	data.cumsum()

	print(data)

	ax = data.plot(x='time', y=['set', 'x'], label=['set value', 'pv'])
	
	ax.set_xlabel("Time (s)")
	ax.set_ylabel("Temperature (°C)")

	ax.minorticks_on()
	ax.grid(which='major', linestyle='-', linewidth='0.5')
	ax.grid(which='minor', linestyle='--', linewidth='0.5')

	ax.set_xlim(0, 1000)
	ax.set_ylim(0, 200)
	
	if len(sys.argv) == 3:
		ax.set_title("Set value vs process value - " + str(sys.argv[2]))
		plt.savefig(str(sys.argv[2]) + '.svg')
	else:
		ax.set_title("Set value vs process value - " + str(sys.argv[3]))
		plt.savefig(str(sys.argv[3]))

	plt.show()
