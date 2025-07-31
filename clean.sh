#!/bin/bash

BASE_DIR="$HOME/.var/app"

# name="mohammad isleem"

# if [ $name = "mohammad isleem" ]; then  # ❌ will break
#     echo "match"
# fi
 
##### difference between [] and [[]]
# if [[ $name = "mohammad isleem" ]]; then  # ✅
    # echo "match"
# fi

if [ -z $BASE_DIR ]; then
  echo "Usage: $0 /path/to/folder"
  exit 1
fi

if [ ! -d "$BASE_DIR" ]; then
  echo "Error: $BASE_DIR is not a directory."
  exit 1
fi

cd "$BASE_DIR" || exit

for folder in */ ; do
	cache_dir="${folder}cache"
	dot_cache_dir="${folder}.cache"

	if [ -d "$cache_dir" ]; then 
		echo "Removing Cache Folder $cache_dir"
		rm -rf $cache_dir
		rm -rf $dot_cache_dir
	elif [ -d "$cache_dot_dir" ]; then 
		echo "Removing cache folder : $cache_dot_dir"
		rm -rf $cache_dot_dor
		echo $cahe_dot_dir
		echo "$cache_dot_dir"
	else
		echo "No cache folders found in $folder"
	fi
done
