Loads and plays /vol/external01/pir.avi.

To make a video file:
```
ffmpeg -i whatever.mp4 -vf scale=1280:720 -c:v mjpeg -q:v 2 -an pir.avi
```
Mess with -q:v (anywhere from 2 (HD) to 31 (needs more jpeg)). Counterintuitively, I find that *lower* numbers (and thus higher JPEG qualities) actually decode faster and result in a marginally higher framerate.

Changing the resolution makes the video render incorrectly; and at 480p it, again, somehow takes *longer* to decode a frame.

Herein comes the rub: The JPEG decoder is painfully slow. According to my perf stats (see `src/monitor.c` and `src/app.c`) the decoder reliably slurps anywhere between 5 and 6 million ticks to decode a single frame, depending on the JPEG quality. That's; uh; too many ticks.

I tweeted a video of this in action [here](https://twitter.com/quarktheawesome/status/925285317686411264). This was with the decoder running at around 5.5-6 million ticks per frame. The clip was *supposed* to be 30s long, but ended up more like 1m10s.

So, yeah. Hopefully you, dear reader, knows what I can do to improve this. Otherwise; this one will go down as another failed Wii U experiement.

(p.s. Apparently delving into libjpeg-turbo code is a [bad idea](https://twitter.com/hedgeberg/status/925301974844788736).)

(p.p.s. I must admit, I have my suspicions. Using an optimal video and *turning off the VSync* only made the video run 5 seconds faster. The math also theorises that at 24fps; we have 50 million ticks available. Could the issue be ProcUI?)
