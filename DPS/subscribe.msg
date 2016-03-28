//srcId wants to receive topic related messages.

message Subscribe_msg
{
    int srcId;//TODO, REALLY NEEDED? the server has the gate, is it sufficient?
    int topic;
};