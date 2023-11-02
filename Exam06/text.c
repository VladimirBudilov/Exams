///Preparations
//headers

//client data stract

//global variables

//error printing


//sand data to all sockets



///main loop
int main(int argc, char *argv[]) {
    //validation

    //create server socket

    //prepare data to work

    //prepare server socket address

    //bind and listen server socket

    ///while loop
    while (1) {
        //update read and write fd by active

        //do select

        //check that socket can read and it is server socket
        if () {
            //accept socket and check on error

            //update max fd

            //set new client id

            //add socket to active

            //send greetings

            break;
        }
        //check that socket can read and it is also client socket
        if () {
            //do recv from client socket to bufferread

            //if recv failed -> error and close connection
            if () {

                break;
            } else {
                //sand data to all clients
                for () {

                }
            }
            break;
        }
    }

    return 0;
}