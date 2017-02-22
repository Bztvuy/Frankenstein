sched_src = ['task.cpp', 'scheduler.cpp', 'taskswitch.S', 'synchronizationevent.cpp']

libsched = static_library('circle_sched', sched_src, include_directories: circle_include)

libsched_dep = declare_dependency(
	include_directories: circle_include,
	link_with: libsched)
