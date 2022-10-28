import matplotlib.pyplot as plt

prev_input = 0; prev_output = 0;
cur_input = 0; cur_output = 0;
tau = 1.2; T = 0.01;

fig = plt.figure()
i = 0
x = list()
y = list()
while i < 500:
    cur_input = 1 #step response
    cur_output = float(prev_input)/(2*float(tau)+float(T))*float(T) + float(cur_input)/(2*float(tau)+float(T))*float(T) - float(prev_output)*(float(T)-2*float(tau))/(2*float(tau)+float(T));
    prev_input = cur_input;
    prev_output = cur_output;

    y.append(cur_output)
    x.append(i*0.01)
    i = i + 1
    if i == 120 :
        print("Response at Time Constant: ", cur_output)
    if i == 240 :
        print("Response at 2 * Time Constant: ", cur_output)
    if i == 360 :
        print("Response at 3 * Time Constant: ", cur_output)
plt.plot(x,y)
plt.show()  

