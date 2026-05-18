#!/usr/bin/env bash

# check wget
if [ ! -x "$(command -v wget)" ]; then
	echo "Please install wget before continuing"
	exit 1
fi

# check whiptail
if [ ! -x "$(command -v whiptail)" ]; then
	echo "Please install whiptail before continuing"
	exit 1
fi

# check unzip
if [ ! -x "$(command -v unzip)" ]; then
	echo "Please install unzip before continuing"
	exit 1
fi

# check make
if [ ! -x "$(command -v make)" ]; then
	echo "Please install make before continuing"
	exit 1
fi

# check grep
if [ ! -x "$(command -v grep)" ]; then
	echo "Please install grep before continuing"
	exit 1
fi

sk_get_march()
{
	if grep -q avx2 /proc/cpuinfo; then
		echo "v3"
	else
		echo "compat"
	fi
}

MARCH=$(sk_get_march)

sk_update_app()
{
	if [ -d "skill-issue" ]; then
		rm -fr "skill-issue"
	fi

	# download our shit
	wget -P skill-issue/ "https://nightly.link/uosq/skill-issue-linux/workflows/main/main/skillissue-$MARCH.zip"
	unzip skill-issue/skillissue-$MARCH.zip -d skill-issue
}

sk_attach()
{
	SO=""

	if [ "$MARCH" = "v3" ]; then
		SO="$PWD/build/x86-64-v3/libvapo.so"
	elif [ "$MARCH" = "compat" ]; then
		SO="$PWD/build/x86-64/libvapo.so"
	fi

	gdb -q -n --batch -p "$PID" \
		-ex "set pagination off" \
		-ex "call (void*)dlopen(\"$SO\", 2)" \
		-ex "call (char*)dlerror()" \
		-ex "detach" \
		-ex "quit"
}

while true; do
	CHOICE=$(whiptail --title "Skill Issue Loader" --menu "What would you like to do?" 16 55 5 \
		"Download"	"Fetch the source code" \
		"Compile"	"Build the application" \
		"Update"	"Fetch changes and recompile" \
		"Attach"	"Run/Attach to the program" \
		"Exit"		"Close this loader" \
		3>&1 1>&2 2>&3)

	# check if pressed cancel or esc
	if [ $? -ne 0 ]; then
		clear
		echo "Exiting loader..."
		exit 0
	fi

	case $CHOICE in
		"Download")
		whiptail --title "Downloading" --infobox "Downloading Skill Issue..." 8 40

		sk_update_app

		whiptail --title "Success" --msgbox "Download complete!" 8 40
		;;

		"Compile")
		whiptail --title "Compiling" --infobox "Compiling the code..." 8 40

		if [ ! -d "skill-issue" ]; then
			whiptail --title "Fail" --msgbox "No skill-issue folder found! Did you forget to download?" 8 40
		else
			cd skill-issue
			make
			cd ..

			whiptail --title "Success" --msgbox "Compilation complete!" 8 40
		fi
		;;

		"Update")
		whiptail --title "Updating" --infobox "Pulling updates and recompiling..." 8 40

		sk_update_app

		whiptail --title "Success" --msgbox "Update complete!" 8 40
		;;

		"Attach")
		whiptail --title "Attaching" --infobox "Attaching Skill Issue" 8 40

		PID=$(pidof -s tf_linux64)

		if [ -z "$PID" ]; then
			whiptail --title "Fail" --msgbox "TF2 is not open!" 8 40
		else
			cd skill-issue

			if sk_attach; then
				whiptail --title "Success" --msgbox "Attached!" 8 40
			else
				whiptail --title "Fail" --msgbox "Failed to attach! (Check terminal for GDB errors)" 8 40
			fi

			cd ..
		fi
		;;

		"Exit")
		clear
		exit 0
		;;
	esac
done