A list of all curently available plugins:

## Infinight
A fixed time delay/looper.

4 fixed lenght delays with each their delay time offset from the others, creating independent patterns that rhytmically interact.

Parameters:
- Input: controls the level of the incomming audio signal.
- Input Algorithm: decides how to handle stereo input.
    - `L only`: only takes left channel
    - `R only`: only takes right channel
    - `L/R -> M/S Mid`: converts stereo input to mid/side and takes the mid channel  
    - `L/R -> M/S Side`: converts stereo input to mid/side and takes the side channel
    - `L/R split -> (1 & 2), (3 & 4)`: splits the left and right channel to delay 1 & 2 and 3 & 4 respectively
    - `L/R split -> (1 & 3), (2 & 4)`: splits the left and right channel to delay 1 & 3 and 2 & 4 respectively
- Delay Time: sets the delay time for the delays.
- Feedback: controls the amount of feedback into the delays.
- Width: controls the panning of the seperate delays.
- Mix: controls the mix of input audio signal and delayed audio signal.
- Output: controls the level of the outgoing audio signal.
