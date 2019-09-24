 local decoder = sjson.decoder()
 
            
            --test1
            --data1='{"channel":{"id":846323,"latitude":"0.0","longitude":"0.0"}}'
            --decoder:write(data1)
            --ret= decoder:result()
            --print(ret["channel"]["id"])
            --print(ret["channel"]["latitude"])


               --test2
            --data1='{"channel":{"id":846323,"latitude":"0.0","longitude":"0.0"},"feeds":[{"created_at":"2019-09-17T16:30:22Z","entry_id":4404,"field1":"1","field2":"2"}]}'
            data1='{"feeds":[{"created_at":"2019-09-17T16:30:22Z","entry_id":4404,"field1":"1","field2":"2"}]}'
            data1='{"channel":{"id":846323,"latitude":"0.0","longitude":"0.0"},"feeds":[{"created_at":"2019-09-17T16:30:22Z","entry_id":4404,"field1":"1","field2":"2"}]}';

            decoder:write(data1)
            ret= decoder:result()
            a =  ret["feeds"][1]["field2"]
           
            print(a..type(a))

            a =  ret["channel"]["latitude"]
           
            print(a..type(a))
         
