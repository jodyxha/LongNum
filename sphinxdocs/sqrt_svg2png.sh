verbose=0

if [ "$#" -gt 1 ] && [ $(( $# % 2)) -eq 0 ]
then

    # get file names
    input=$1
    output=$2

    shift 2

    #set defaults

    # default x-offset to outer border
    bw=35
    # default y-offset to outer border
    bh=35
    # default outer border width
    bb=4

    # default background color
    col="rgb(192,192,192)"
    # default outer border color
    xcol="rgb(0,0,0)"

    # get parameters
    while (( "$#" ))
      do
      if [ "$1" == "-bw" ]
          then
          bw=$2
      elif [ "$1" == "-bh" ]
          then
          bh=$2
      elif [ "$1" == "-bb" ]
          then
          bb=$2
      elif [ "$1" == "-col" ]
          then
          col=$2
      elif [ "$1" == "-xcol" ]
          then
          xcol=$2
      fi
      shift
    done

    #show parameters
    if [ 1 == 1 ]
    then
        echo "input file:         $input"
        echo "output file:        $output"
        echo "bgborder w:         $bw"
        echo "bgborder h:         $bh"
        echo "outer border w:     $bb"
        echo "bg color:           $col"
        echo "outer border color: $xcol"
    fi

    if [ $verbose == 1 ]; then echo "load and trim"; fi
    # load and trim
    convert -background none $input -trim  temp1.png

    if [ $verbose == 1 ]; then echo "get image size"; fi
    # get image size
    wh=`magick identify temp1.png | gawk '{ print $3 }'`
    IFS='x' read -ra my_array <<< "$wh"

    w0=${my_array[0]}
    h0=${my_array[1]}

    w1=`expr ${w0} + 2 \* ${bw}`
    h1=`expr ${h0} + 2 \* ${bh}`

    if [ 1 == 1 ]; then echo "origsize ${w0} x ${h0}, new size ${w1} x ${h1}"; fi

    if [ $verbose == 1 ]; then echo "add background"; fi
    
    magick -size ${w0}x${h0} xc:$col \( -page +0+0 temp1.png \) -layers flatten  temp2.png
    #magick -size ${w0}x${h0} xc:'rgb(192,192,192)' \( -page +0+0 temp1.png \) -layers flatten  temp2.png

    if [ $verbose == 1 ]; then echo "expand canvas"; fi
    convert temp2.png -gravity center -background $col -extent ${w1}x${h1} temp3.png

    if [ $verbose == 1 ]; then echo "add outer border"; fi
    magick temp3.png  -bordercolor $xcol -border $bb  $output

    if [ $verbose == 1 ]; then echo "cleanup"; fi
    rm temp1.png temp2.png temp3.png

else
  echo "usage"
  echo "$0 <input_svg> <output_png> [-bw <bgborder_width>] [-bh <bgborder_height>]  [-bb <outerborder_width>]  "
  echo "        [-col <bgcolor>] [-xcol <outerborder_color>]  "
fi