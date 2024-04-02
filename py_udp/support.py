import datetime




class ExternalHelper:
    @staticmethod
    def build_byte_array():
        now = datetime.datetime.now()

        byte_array = [
            0xaa,
            0x81,
            now.second,
            now.minute,
            now.hour,
            now.day,
            now.month,
            0,
            now.year-2000,
            0,
            now.weekday()
        ]

        return byte_array