import matplotlib.pyplot as plt
plt.style.use('_mpl-gallery')

def strip_brackets(raw_str):
	raw_str = raw_str.replace("[", "")
	raw_str = raw_str.replace("]", "")
	return raw_str;
	
def prep_list(raw_str):
	raw_list = raw_str.split(",")
	for i in range(len(raw_list)):
		raw_list[i] = int(raw_list[i])
	return raw_list

raw_x = input("X coordinate values: ")
raw_y = input("Y coordinate values: ")

raw_x = strip_brackets(raw_x)
raw_y = strip_brackets(raw_y)

# plot
x = prep_list(raw_x)
y = prep_list(raw_y)

plt.figure(figsize=(8, 8), dpi=80)
plt.plot(y, x, marker='x', mec = 'red', mfc='red', markersize = 10.0)
plt.axis('square')
plt.tight_layout()
plt.show()
