import numpy as np
def tofmass(wt):
    masses = np.array([2,18])
    time_in_ns = np.array(wt)
    mass_rt = np.sqrt(masses)
    # mass_rt = masses
    fitp = np.polyfit(time_in_ns, mass_rt, 1)
    fito = np.poly1d(fitp)
    # print(fito(4255)**2)
    print("intercept:\t",fito[0],"\tslope:\t",fito[1])
    return fitp
hydrogentime = 845
watertime = 2540
tofmass(np.array([hydrogentime,watertime]))
# print(tofmass(np.array([2,18])))

