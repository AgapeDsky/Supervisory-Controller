import matplotlib.pyplot as plt

prev_input = 0; prev_output = 0;
cur_input = 0; cur_output = 0;
tau = 1.2; T = 0.01;
kp = 5;

target_vel = 0;
dist = 0;

def compute():
    retVal = prev_input/(2*tau+T)*T + cur_input/(2*tau+T)*T - prev_output*(T-2*tau)/(2*tau+T);
    return retVal

fig = plt.figure()
i = 0
x = list()
y = list()
distance = list()

while i < 7:
    dist = dist + cur_output*T
    target_vel = target_vel + 0.15
    cur_input = (target_vel - cur_output)*kp 
    cur_output = compute()
    prev_input = cur_input
    prev_output = cur_output

    y.append(cur_output)
    x.append(i*T)
    distance.append(dist)
    i = i + 1

j = 0
while j < 150:
    dist = dist + cur_output*T
    target_vel = target_vel
    cur_input = (target_vel - cur_output)*kp 
    cur_output = float(prev_input)/(2*float(tau)+float(T))*float(T) + float(cur_input)/(2*float(tau)+float(T))*float(T) - float(prev_output)*(float(T)-2*float(tau))/(2*float(tau)+float(T));
    prev_input = cur_input;
    prev_output = cur_output;

    y.append(cur_output)
    x.append((i+j)*T)
    distance.append(dist)
    j = j + 1

k = 0
while k < 7:
    dist = dist + cur_output*T
    target_vel = target_vel - 0.15
    cur_input = (target_vel - cur_output)*kp 
    cur_output = float(prev_input)/(2*float(tau)+float(T))*float(T) + float(cur_input)/(2*float(tau)+float(T))*float(T) - float(prev_output)*(float(T)-2*float(tau))/(2*float(tau)+float(T));
    prev_input = cur_input;
    prev_output = cur_output;

    y.append(cur_output)
    x.append((i+j+k)*T)
    distance.append(dist)
    k = k + 1

m = 0
while m < 200:
    dist = dist + cur_output*T
    target_vel = 0
    cur_input = (target_vel - cur_output)*kp 
    cur_output = float(prev_input)/(2*float(tau)+float(T))*float(T) + float(cur_input)/(2*float(tau)+float(T))*float(T) - float(prev_output)*(float(T)-2*float(tau))/(2*float(tau)+float(T));
    prev_input = cur_input;
    prev_output = cur_output;

    y.append(cur_output)
    x.append((i+j+k+m)*T)
    distance.append(dist)
    m = m + 1


a = 0
while a < 7:
    dist = dist + cur_output*T
    target_vel = target_vel - 0.15
    cur_input = (target_vel - cur_output)*kp 
    cur_output = compute()
    prev_input = cur_input
    prev_output = cur_output
    prev_input = cur_input;
    prev_output = cur_output;

    y.append(cur_output)
    x.append((i+j+k+m+a)*T)
    distance.append(dist)
    a = a + 1

b = 0
while b < 150:
    dist = dist + cur_output*T
    target_vel = target_vel
    cur_input = (target_vel - cur_output)*kp 
    cur_output = float(prev_input)/(2*float(tau)+float(T))*float(T) + float(cur_input)/(2*float(tau)+float(T))*float(T) - float(prev_output)*(float(T)-2*float(tau))/(2*float(tau)+float(T));
    prev_input = cur_input;
    prev_output = cur_output;

    y.append(cur_output)
    x.append((i+j+k+m+a+b)*T)
    distance.append(dist)
    b = b + 1

c = 0
while c < 7:
    dist = dist + cur_output*T
    target_vel = target_vel + 0.15
    cur_input = (target_vel - cur_output)*kp 
    cur_output = float(prev_input)/(2*float(tau)+float(T))*float(T) + float(cur_input)/(2*float(tau)+float(T))*float(T) - float(prev_output)*(float(T)-2*float(tau))/(2*float(tau)+float(T));
    prev_input = cur_input;
    prev_output = cur_output;

    y.append(cur_output)
    x.append((i+j+k+m+a+b+c)*T)
    distance.append(dist)
    c = c + 1
d = 0
while d < 200:
    dist = dist + cur_output*T
    target_vel = 0
    cur_input = (target_vel - cur_output)*kp 
    cur_output = float(prev_input)/(2*float(tau)+float(T))*float(T) + float(cur_input)/(2*float(tau)+float(T))*float(T) - float(prev_output)*(float(T)-2*float(tau))/(2*float(tau)+float(T));
    prev_input = cur_input;
    prev_output = cur_output;

    y.append(cur_output)
    x.append((i+j+k+m+a+b+c+d)*T)
    distance.append(dist)
    d = d + 1


plt.plot(x,y)
plt.plot(x, distance)
plt.title('Distance and Velocity')
plt.xlabel('Time Stamp (s)')
plt.ylabel('Distance: Yellow, Velocity: Blue')
plt.show()  
