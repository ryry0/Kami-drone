#! /bin/bash

tmux list-panes > /tmp/tmux.list-panes
awk '{if ($1!="0:") {exit 1}}' /tmp/tmux.list-panes
status=$?
 
# if exists -> kill, else -> do nothing
if [ ${status} == 1 ]; then
	tmux kill-pane -a -t 0
fi

if make && make upload
then
tmux split-window -h
#tmux send-keys './tools/read_data.x' 'C-m'
tmux send-keys './tools/read_all_data.x; pkill write_serial*; tmux kill-pane -t 2; exit' 'C-m'

tmux split-window -v
tmux send-keys './tools/write_serial.x' 'C-m'
fi
