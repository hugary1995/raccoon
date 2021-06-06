import csv
import pandas as pd
from enum import Enum
import matplotlib.pyplot as plt


class STATE(Enum):
    FULL = 1
    PARTIAL = 2
    SHUTDOWN = 3
    FP = 4
    PF = 5
    FS = 6
    SF = 7


filename = 'BC.csv'

# all time in seconds
full_duration = 240 * 3600
partial_duration = 0 * 3600
shutdown_duration = 25 * 3600
fp_transition = 0 * 3600
pf_transition = 0 * 3600
fs_transition = 6 * 3600
sf_transition = 3 * 3600
cycle_duration = full_duration + fp_transition + partial_duration + pf_transition
shutdown_cycle_duration = fs_transition + shutdown_duration + sf_transition

# schedule:
# full -> fp -> partial -> pf
#    <---------------------|
# after each pf transition, check if we need to shutdown
# schedule for shutdown:
# fs -> shutdown -> sf

# operation time until shutdown: 6 months
operation_duration = 3 * 30 * 24 * 3600

# total simulation time
total_duration = 6 * 12 * 30 * 24 * 3600

# timestep
full_dt = full_duration / 1
partial_dt = partial_duration / 1
shutdown_dt = shutdown_duration / 1
fp_dt = fp_transition / 1
pf_dt = pf_transition / 1
fs_dt = fs_transition / 9
sf_dt = sf_transition / 9

# all temperature in celsius
# T_gas_full = 1100
T_gas_full = 900
T_gas_partial = 845
T_gas_shutdown = 25
# T_steam_full = 530
T_steam_full = 525
T_steam_partial = 525
T_steam_shutdown = 25

# all pressure in MPa
p_gas_full = 0.2
p_gas_partial = 0.1
p_gas_shutdown = 0.1
p_steam_full = 17
p_steam_partial = 10
p_steam_shutdown = 0.1


def interpolate(start, end, fraction):
    return start + (end - start) * fraction


def get_cycle_state(current_time_in_cycle):
    if current_time_in_cycle < full_duration:
        state = STATE.FULL
    elif current_time_in_cycle < full_duration + fp_transition:
        state = STATE.FP
    elif current_time_in_cycle < full_duration + fp_transition + partial_duration:
        state = STATE.PARTIAL
    elif current_time_in_cycle < full_duration + fp_transition + partial_duration + pf_transition:
        state = STATE.PF
    else:
        state = STATE.FULL
    return state


def get_shutdown_cycle_state(current_time_in_cycle):
    if current_time_in_cycle < fs_transition:
        state = STATE.FS
    elif current_time_in_cycle < fs_transition + shutdown_duration:
        state = STATE.SHUTDOWN
    elif current_time_in_cycle < fs_transition + shutdown_duration + sf_transition:
        state = STATE.SF
    else:
        state = STATE.FULL
    return state


def get_dt(current_state):
    if current_state == STATE.FULL:
        dt = full_dt
    elif current_state == STATE.PARTIAL:
        dt = partial_dt
    elif current_state == STATE.SHUTDOWN:
        dt = shutdown_dt
    elif current_state == STATE.FP:
        dt = fp_dt
    elif current_state == STATE.PF:
        dt = pf_dt
    elif current_state == STATE.FS:
        dt = fs_dt
    elif current_state == STATE.SF:
        dt = sf_dt
    return dt


def get_cycle_bcs(current_time_in_cycle, current_state):
    if current_state == STATE.FULL:
        T_gas = T_gas_full
        T_steam = T_steam_full
        p_gas = p_gas_full
        p_steam = p_steam_full
    elif current_state == STATE.FP:
        fraction = (current_time_in_cycle - full_duration) / fp_transition
        T_gas = interpolate(T_gas_full, T_gas_partial, fraction)
        T_steam = interpolate(T_steam_full, T_steam_partial, fraction)
        p_gas = interpolate(p_gas_full, p_gas_partial, fraction)
        p_steam = interpolate(p_steam_full, p_steam_partial, fraction)
    elif current_state == STATE.PARTIAL:
        T_gas = T_gas_partial
        T_steam = T_steam_partial
        p_gas = p_gas_partial
        p_steam = p_steam_partial
    elif current_state == STATE.PF:
        fraction = (current_time_in_cycle - full_duration -
                    fp_transition - partial_duration) / pf_transition
        T_gas = interpolate(T_gas_partial, T_gas_full, fraction)
        T_steam = interpolate(T_steam_partial, T_steam_full, fraction)
        p_gas = interpolate(p_gas_partial, p_gas_full, fraction)
        p_steam = interpolate(p_steam_partial, p_steam_full, fraction)
    return [str(T_gas), str(T_steam), str(p_gas), str(p_steam)]


def get_shutdown_cycle_bcs(current_time_in_cycle, current_state):
    if current_state == STATE.FS:
        fraction = current_time_in_cycle / fs_transition
        T_gas = interpolate(T_gas_full, T_gas_shutdown, fraction)
        T_steam = interpolate(T_steam_full, T_steam_shutdown, fraction)
        p_gas = interpolate(p_gas_full, p_gas_shutdown, fraction)
        p_steam = interpolate(p_steam_full, p_steam_shutdown, fraction)
    elif current_state == STATE.SHUTDOWN:
        T_gas = T_gas_shutdown
        T_steam = T_steam_shutdown
        p_gas = p_gas_shutdown
        p_steam = p_steam_shutdown
    elif current_state == STATE.SF:
        fraction = (current_time_in_cycle - fs_transition -
                    shutdown_duration) / sf_transition
        T_gas = interpolate(T_gas_shutdown, T_gas_full, fraction)
        T_steam = interpolate(T_steam_shutdown, T_steam_full, fraction)
        p_gas = interpolate(p_gas_shutdown, p_gas_full, fraction)
        p_steam = interpolate(p_steam_shutdown, p_steam_full, fraction)
    elif current_state == STATE.FULL:
        T_gas = T_gas_full
        T_steam = T_steam_full
        p_gas = p_gas_full
        p_steam = p_steam_full
    return [str(T_gas), str(T_steam), str(p_gas), str(p_steam)]


def write_cycle(writer, current_time):
    dt = full_dt
    current_time_in_cycle = dt
    while current_time_in_cycle <= cycle_duration:
        current_state = get_cycle_state(current_time_in_cycle)
        bcs = get_cycle_bcs(current_time_in_cycle, current_state)
        dt = get_dt(current_state)
        writer.writerow(
            [str(current_time + current_time_in_cycle), str(dt), current_state.value] + bcs)
        current_time_in_cycle += dt


def write_shutdown_cycle(writer, current_time):
    dt = fs_dt
    current_time_in_cycle = dt
    while current_time_in_cycle <= shutdown_cycle_duration:
        current_state = get_shutdown_cycle_state(current_time_in_cycle)
        bcs = get_shutdown_cycle_bcs(current_time_in_cycle, current_state)
        dt = get_dt(current_state)
        writer.writerow(
            [str(current_time + current_time_in_cycle), str(dt), current_state.value] + bcs)
        current_time_in_cycle += dt


with open(filename, 'w', newline='') as csvfile:
    csvwriter = csv.writer(csvfile, delimiter=',',
                           quotechar='|', quoting=csv.QUOTE_MINIMAL)
    csvwriter.writerow(
        ['time', 'dt', 'state', 'T_gas', 'T_steam', 'p_gas', 'p_steam'])
    current_time = 0
    # start with full
    csvwriter.writerow([str(current_time), str(full_dt), STATE.FULL.value, str(
        T_gas_full), str(T_steam_full), str(p_gas_full), str(p_steam_full)])

    while current_time < total_duration:
        operation_time = 0
        while operation_time < operation_duration:
            write_cycle(csvwriter, current_time)
            operation_time += cycle_duration
            current_time += cycle_duration
        # write shutdown
        write_shutdown_cycle(csvwriter, current_time)
        current_time += shutdown_cycle_duration

data = pd.read_csv(filename)
plt.plot(data['time'], data['T_gas'], 'o-')
plt.show()
