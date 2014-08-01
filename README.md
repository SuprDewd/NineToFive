
NineToFive
==========

NineToFive is a lightweight command-line application for keeping track of your
work hours. All of its functionality can be found in the `work` executable,
which helps you log your activity while you work.

Installation
------------
```
$ ./configure
$ make
# make install
```

Usage
-----
NineToFive keeps an event log, where each event has a description (such as
"started working") and a timestamp. The `work` executable can be used as a
frontend to the event log.

work
----
The main `work` executable.

work start
----------
The `work start` command appends a **start** event to the event log with the
current time as a timestamp.

work stop
---------
The `work stop` command appends a **stop** event to the event log with the
current time as a timestamp.

work since
----------
The `work since TIME` command appends a **start** event to the event log with a
timestamp derived from `TIME`. The `TIME` variable can be of the following
forms:

- `X`, meaning at `X` o'clock. If this is ambiguous then the last possible time is used
- `X:Y`, meaning at `Y` minutes past `X` o'clock. If this is ambiguous then the last possible time is used
- `Xm`, meaning `X` minutes ago
- `Xh`, meaning `X` hours ago
- `X:Yh`, meaning `X` hours and `Y` minutes ago

Note that specifying seconds in a similar manner (possibly with the `s` suffix)
is also supported, but is omitted here for simplicity.

### Examples
```
work since 9      # I've been working since 9:00 this morning
work since 9:15   # I've been work since 9:15 this morning
work since 2:20h  # I started working 2 hours and 20 minutes ago
```

work at
-------
The `work at TIME` command appends a **start** event to the event log with a
timestamp derived from `TIME`. The `TIME` variable can be of the following
forms:

- `X`, meaning at `X` o'clock. If this is ambiguous then the next possible time is used
- `X:Y`, meaning at `Y` minutes past `X` o'clock. If this is ambiguous then the next possible time is used
- `Xm`, meaning after `X` minutes
- `Xh`, meaning after `X` hours
- `X:Yh`, meaning after `X` hours and `Y` minutes

Note that specifying seconds in a similar manner (possibly with the `s` suffix)
is also supported, but is omitted here for simplicity.

It has the following options:

- `--notify` will generate a notification at the moment you should start working (according to `TIME`)
- `--execute CMD` will execute `CMD` at the moment you should start working (according to `TIME`)

### Examples
```
work at 12        # I'm going to start working at noon
work at 12:15     # I'm going to start working at quarter past noon
work at 2:20h     # I'm going to start working in 2 hours and 20 minutes
work at 2:20h --notify
work at 2:20h --run 'notify-send "Get to work!"'
```

work until
----------
TODO: work until a given time with support for notifications like `work at`. Should it support negative time?

work for
--------
TODO: work for some given amount of time with support for notifications like `work at`. Should it support negative time? Maybe merge with `work until`?

work on
-------
TODO: add a **on XXX** event to the event log, which is a hint/tag for *what* you are working on. Possibly also support `--on` options for other commands.

work while
----------
The `work while CMD` command appends a **start** event to the event log with the
current time as a timestamp, then executes `CMD` and waits for it to exit, and
finally appends a **stop** event to the log with the time when `CMD` ended as a
timestamp. This is equivalent to running the following:

```
$ work start && (CMD || true) && work stop
```

### Examples
```
work while bash        # Run a bash shell where I do my work
work while "sleep 60"  # An alternative way to work for 1 minute
```

work merge
----------
The ```work merge LOG1 LOG2 LOG3...``` command takes zero or more NineToFive
event logs and combines them into one. It has the following options:

- `--output FILE` specifies where to write the output. By default `FILE` is `-`, which represents the standard output.

work invoice
------------
TODO: somehow export data or generate a LaTeX file which can be used to generate an invoice. Only work hours since the last invoice was created (i.e. the last **invoice** event) will be used (so add **stop**, **invoice**, **start** events in that order).

TODO
====

- Decide if we should merge `since`/`at` into `start` and `until`/`for` into
  `stop` with some super syntax for specifying TIME. It's probably enough to
  have a `+` and `-` prefix depending on what the user wants to use.

- Support multiple event logs in a clean way.

- Timestamps in the event log should have a timezone, so that commands like
  `work merge` work correctly on logs from multiple machines.

