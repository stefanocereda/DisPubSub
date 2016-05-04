using DataFrames

client_df = readtable("./data_client.csv")
broker_df = readtable("./data_broker.csv")

client_agg = aggregate(client_df, [:client], [mean, std])
broker_agg = aggregate(broker_df, [:broker], [mean, std])

client_file = open("./agg_client.txt", "w")
write(client_file, repr(client_agg[1,:]))
close(client_file)

broker_file = open("./agg_broker.txt", "w")
write(broker_file, repr(broker_agg[1,:]))
close(broker_file)
