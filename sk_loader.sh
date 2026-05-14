#!/usr/bin/env bash

#

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
		whiptail --title "Downloading" --infobox "\nDownloading Skill Issue..." 8 40

		# download our shit
		wget -P skill-issue/ https://nightly.link/uosq/skill-issue-linux/workflows/main/main/skillissue-v3.zip
		unzip skill-issue/skillissue-*.zip -d skill-issue

		whiptail --title "Success" --msgbox "Download complete!" 8 40
		;;

		"Compile")
		whiptail --title "Compiling" --infobox "\nCompiling the code..." 8 40

		if [ ! -d "skill-issue" ]; then
			whiptail --title "Success" --msgbox "No skill-issue folder found! Did you forget to download?" 8 40
		else
			cd skill-issue
			make

			whiptail --title "Success" --msgbox "Compilation complete!" 8 40
		fi
		;;

		"Update")
		whiptail --title "Updating" --infobox "\nPulling updates and recompiling..." 8 40



		whiptail --title "Success" --msgbox "Update complete!" 8 40
		;;

		"Attach")
		whiptail --title "Attaching"
		clear
		echo "Attaching to program..."

		exit 0
		;;

		"Exit")
		clear
		exit 0
		;;
	esac
done