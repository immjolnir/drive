# IO

```
ZeroCopyInputStream
  |
  |_ ZeroCopyCodedInputStream

EpsCopyInputStream
  |
  |_ ParseContext

```
- src/google/protobuf/parse_context.h
ParseContext holds all data that is global to the entire parse.
Most importantly it contains the input stream, but also recursion depth and also
stores the end group tag, in case a parser ended on a endgroup, to 
verify matching start/end group tags.