import glob
import os


def resolve_video_device(vendor_id: str, product_id: str) -> str:
    """Find the /dev/videoN node belonging to a USB camera by VID:PID.

    /dev/videoN numbering is assigned in USB probe order at boot, which is
    not guaranteed stable across boots/reboots when multiple cameras of the
    same class are attached - the same physical camera can land on a
    different node number from one boot to the next. VID:PID is burned into
    the camera's USB descriptor and stays fixed regardless of enumeration
    order, so it's used here instead of a hardcoded node number.
    """
    vendor_id = vendor_id.strip().lower()
    product_id = product_id.strip().lower()

    for v4l_node in sorted(glob.glob('/sys/class/video4linux/video*')):
        usb_dir = os.path.realpath(os.path.join(v4l_node, 'device'))
        while True:
            vid_file = os.path.join(usb_dir, 'idVendor')
            pid_file = os.path.join(usb_dir, 'idProduct')
            if os.path.isfile(vid_file) and os.path.isfile(pid_file):
                with open(vid_file) as f:
                    found_vid = f.read().strip().lower()
                with open(pid_file) as f:
                    found_pid = f.read().strip().lower()
                if found_vid == vendor_id and found_pid == product_id:
                    return '/dev/' + os.path.basename(v4l_node)
                break
            parent = os.path.dirname(usb_dir)
            if parent == usb_dir:
                break
            usb_dir = parent

    raise RuntimeError(
        f"No V4L2 device found for USB {vendor_id}:{product_id} "
        f"- check the camera is connected"
    )
