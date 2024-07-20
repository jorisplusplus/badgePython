import esp32, gc


def get_int(namespace="system", key=None):
    nvs = esp32.NVS(namespace)
    result = None
    try:
        result = nvs.get_i32(key)
    except OSError:
        # Thrown when key doesn't exist
        pass
    finally:
        del nvs
        gc.collect()

    return result


def set_int(namespace="system", key=None, value=None):
    nvs = esp32.NVS(namespace)
    result = None
    try:
        result = nvs.set_i32(key, value)
        nvs.commit()
    finally:
        del nvs
        gc.collect()

    return result


def get_str(namespace="system", key=None):
    nvs = esp32.NVS(namespace)
    result = None
    try:
        len = nvs.get_blob(key, bytearray())
        result = bytearray(len)
        nvs.get_blob(key, result)
    except OSError:
        # Thrown when key doesn't exist
        pass
    finally:
        del nvs
        gc.collect()

    return result.decode('ascii') if result is not None else None


def set_str(namespace="system", key=None, value=None):
    nvs = esp32.NVS(namespace)
    result = None
    try:
        nvs.set_blob(key, value)
        nvs.commit()
    finally:
        del nvs
        gc.collect()
