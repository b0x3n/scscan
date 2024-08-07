# scscan

Simple c port scanner.

I found it! Not as interesting as I remember and I made a few tweaks to the code, added some comments, etc.

I need to spend a bit of time documenting the available command-line options, and there are quite a few!


## Basic operation

We can scan lists or ranges of host IP addresses. First, if we want to scan a list of hosts (we can use domans or IP addresses:

```
	scscan --host www.google.com --host www.youtube.com
```

## Scanning host and port ranges

This will scan a default port range (1 - 10,000) for each of the given hosts. If we want to scan a range of IP/domans we specify the -hrange option:

```
	scscan --host 1.2.3.4 --host 2.3.4.5 -hrange
```

Specifying ports is similar, we can specify a list of individual ports:

```
	scscan --host 1.2.3.4 --port 25 --port 80
```

Or we can specify a range of ports usng the -prange switch:

```
	scscan --host 1.2.3.4 --port 25 --port 80 -prange
```

## Inversion

Both host and port lists/ranges can be inverted. For example, if we want to scan all hosts except 1.2.3.4:

```
	scscan --host 1.2.3.4 -invert-hosts
```

And similarly, we can use the -invert-ports switch to scan all ports except:

```
	scscan --host 1.2.3.4 --port 25 --port 80 -invert-ports
```

## Colo(u)r output

You can also use the -color switch to enable coloured output to the terminal:

```
	scscan --host 1.2.3.4 -color
```

## Other options

Some other options include:

```
	--out - specify an output file (other than stdout)
	--err - specify an error file (other than stderr)
```

Both of the above options allow you to specify a filename beginning with the + character (open for appending)


## Wrap up

I wrote a few different versions of this. I think this was the most robust version with the built-in range scanning functionality - interacting with the ports DB to show service names and so on. In retrospect it's nice, but not perfect. I'd like to rewrite this and possibly introduce some new, more advanced scanning methods - half open, etc.

We shall see! I have other gigs on the go so this isn't a priority but it was fun to write.


Hopefully someone finds it useful or can learn something from the (not perfect) code.


Michael.
