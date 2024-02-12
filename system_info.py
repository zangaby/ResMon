import psutil
import socket
import serial
import time

def get_cpu_load():
    return psutil.cpu_percent(interval=1)

def get_memory_usage():
    return psutil.virtual_memory().used / (1024 ** 2)

def get_ip_address(interface="enp2s0"):
    try:
        # Retrieve the IP address associated with the specified interface
        addresses = psutil.net_if_addrs()
        ip_address = addresses[interface][0].address
        return ip_address
    except (KeyError, IndexError, psutil.Error) as e:
        print(f"Error retrieving IP address: {e}")
        return "N/A"

def main():
    ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)

    while True:
        try:
            cpu_load = get_cpu_load()
            memory_usage = get_memory_usage()
            ip_address = get_ip_address(interface="enp2s0")  # Specify the desired interface

            # Change the format to "cpuLoad, memoryUsage, ipAddress\n"
            data = f"{cpu_load},{memory_usage},{ip_address}\n"
            ser.write(data.encode('utf-8'))

            time.sleep(2)  # Adjust sleep duration based on your desired data update rate

        except KeyboardInterrupt:
            ser.close()
            break

if __name__ == "__main__":
    main()

