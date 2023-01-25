#!/usr/bin/env python
# encoding: utf-8

###############################################################################

from waflib.TaskGen import extension, feature
from waflib import Task

import sys

###############################################################################

def configure(cfg):
	cfg.find_program('avrdude', var = 'AVRDUDE')
	cfg.env.AVRDUDEFLAGS = [
		'-p', cfg.env.MCU,
		'-c', cfg.env.PROGRAMMER
	]
	
	#TODO Take cfg.env.FREQ in consideration.
	#freq_kHz = 100
	#FIXME Need long time and MCU stay in reset.
	#if cfg.env.PROGRAMMER == 'dragon_jtag':
	#	# With this it exists without error.
	#	cfg.env.AVRDUDEFLAGS += ['-B', '{}kHz'.format(freq_kHz)]
	
	if cfg.env.PROGRAMMER == 'arduino':
		#cfg.env.AVRDUDEFLAGS += ['-b', '115200']
		if cfg.env.PROGRAMMER_PORT != []:
			cfg.env.AVRDUDEFLAGS += ['-P', cfg.env.PROGRAMMER_PORT]
		else:
			if sys.platform.startswith('linux'):
				cfg.env.AVRDUDEFLAGS += ['-P', '/dev/ttyUSB0']
			else:
				cfg.env.AVRDUDEFLAGS += ['-P', 'COM2']
	
	#cfg.env.AVRDUDEFLAGS += ['-vv']

class avrdude_read_fuses_task(Task.Task):
	def run(self):
		cmd = self.env.AVRDUDE
		cmd += self.env.AVRDUDEFLAGS
		#TODO Pipe it. Parse it. Process nice.
		#cmd.append('-Uhfuse:r:-:b')
		#cmd.append('-Ulfuse:r:-:b')
		ret = self.exec_command2(cmd)
		if ret:
			return ret

@feature('avrdude_read_fuses')
def avrdude_read_fuses_feature(tg):
	t = tg.create_task('avrdude_read_fuses_task', None, None)
	t.always_run = True


class avrdude_upload(Task.Task):
	def run(self):
		for src in self.inputs:
			cmd = self.env.AVRDUDE
			cmd += self.env.AVRDUDEFLAGS
			cmd.append('-Uflash:w:{}:i'.format(src.relpath()))
			ret = self.exec_command2(cmd)
			if ret:
				return ret
	
	def runnable_status(self):
		ret = super(avrdude_upload, self).runnable_status()
		if ret == Task.SKIP_ME:
			return Task.RUN_ME
		return ret


@extension('.hex')
def avrdude_hex_hook(tg, node):
	t = tg.create_task('avrdude_upload', node, None)

###############################################################################
