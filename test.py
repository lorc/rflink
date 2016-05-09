import usb1
import time
def main():
    
    with usb1.USBContext() as context:
        handle = context.openByVendorIDAndProductID(
            0x146,
            1,
            skip_on_error=True,
        )
        if handle is None:
            print("No device found")
            return
        handle.setConfiguration(1)
        with handle.claimInterface(0):
            print(handle)
            for x in range(32):
                data = b"1234"
                handle.bulkWrite(0x1, data)
                din = handle.bulkRead(0x81, 4)
                print(din)
            # Do stuff with endpoints on claimed interface.
if __name__ == "__main__":
    main()
