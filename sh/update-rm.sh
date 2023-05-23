#!/usr/bin/env bash

ROOT_README="README.md"
AUTHOR=$(whoami)
CREATOR="$0"
CREATOR=${CREATOR[@]/\.\//}
TIME_STAMP=$(date "+%m%d%Y - %T")


add_meta_data() {
  echo -e "\
---
author:   $AUTHOR
creator:  $CREATOR
date:     $TIME_STAMP
---

# gtk4 playground

This is a playground for gtk4. It is a collection of projects, examples, and information about developing gtk4 applications.

## Projects
" > $ROOT_README

}

add_project_table(){
  # Create a list of all the README.md files in the proj directory
  proj_readmes=$(find proj -name "README.md")
  # Create a list of all the preview.png files in the proj directory
  proj_previews=$(find proj -name "preview.png")


  echo -e "\
| Name | Description | Preview |
| ---- | ----------- | ------- |" >> $ROOT_README

  for readme in $proj_readmes; do
    for preview in $proj_previews; do
      if [ -n "$readme" ]; then

        # Get the name of the project from the yaml front matter with sed
        front_matter=$(sed -n '/^---$/,/^---$/p' "$readme")

        # Get the name of the project from the yaml front matter with grep
        name=$(echo "$front_matter" | grep -oP '(?<=name: ).*')
        description=$(echo "$front_matter" | grep -oP '(?<=description: ).*')
        echo -e "| [$name]($readme) | $description | ![$name]($preview) | " >> $ROOT_README
      fi
    done
  done
}

add_meta_data
add_project_table
