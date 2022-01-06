# ct-comp submission file
Models with no constraints

## build

On UNIX or linux, run below command.(require `cmake` and `gcc` )

```
make
./IPOsolver (strength) (input_file_name)
```

On Docker, run below command.(require `docker` and `docker-compose` )

```
docker-compose up -d
docker cp (input_file_path) k_takemr_iposolver:/app/(input_file_name)
docker exec k_takemr_iposolver sh
./IPOsolver (strength) (input_file_name)
^P^Q
docker cp k_takemr_iposolver:(output_file_name) (output_file_path)
```

`(output_file_name)` is the test case name + '.csv'.
If a test case name is 'MCA_0', `(output_file_name)` is 'MCA_0.csv'. 