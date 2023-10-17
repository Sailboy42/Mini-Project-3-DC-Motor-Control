import serial
import pandas as pd


def serial2_csv(arduino_com_port="COM3", baud_rate=9600, file_path="serial_data.csv"):
    """
    Saves data being sent over serial to a csv. Designed for arduino usage. Will check that file does not exist and prompt user to choose a new file name if so.

    Args:
        arduino_com_port: a string dictating what com port the device is attached to
        baud_rate: a int dictatating teh baudRate of serial transfer
        file_path: a string of the name of the file including its extension

    Returns:
        NA
    """
    serial_port = serial.Serial(arduino_com_port, baud_rate, timeout=1)
    lf_data = pd.DataFrame()

    while True:
        #
        # ask for a line of data from the serial port, the ".decode()" converts the
        # data from an "array of bytes", to a string
        #
        scan_data = serial_port.readline().decode()

        #
        # check if data was received
        #
        if len(scan_data) > 0:
            # print(scan_data)
            scan_data = list(scan_data.split(","))
            scan_data = [x.rstrip() for x in scan_data]
            # print(type(scan_data))
            print(scan_data)
            df = pd.DataFrame(scan_data)  # convert list to dataframe
            print(df)
            lf_data = lf_data.append(df, ignore_index=True)

        lf_data.to_csv(lf_data, sep=",", index=False, encoding="utf-8")

        """***************************************************************************************
        *    Title: Pandas tocsv Checking for Overwrite Using Mode Paramter
        *    Author: Saturn Cloud
        *    Date: Monday, June 19, 2023
        *    Code version: 1
        *    Availability: https://saturncloud.io/blog/pandas-tocsv-checking-for-overwrite/
        *
        ***************************************************************************************"""
        """
        try:
            df.to_csv(file_path, index=False, mode="x")
            print(f"The file '{file_path}' has been created.")
        except FileExistsError:
            overwrite = input(
                f"The file '{file_path}' already exists. Do you want to overwrite it? (y/n): "
            )
            if overwrite.lower() == "y":
                df.to_csv(file_path, index=False, mode="w")
                print(f"The file '{file_path}' has been overwritten.")
            else:
                print(f"The file '{file_path}' has not been overwritten.")
        """
    return


serial2_csv(arduino_com_port="COM7", file_path="serial_data_test.csv")
