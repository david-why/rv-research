module matmul_accelerator #(
    parameter DATA_WIDTH = 32,
    parameter CHUNK_WIDTH = 8,
    parameter CHUNK_HEIGHT = 8
) (
    input wire [DATA_WIDTH-1:0] row_a [0:CHUNK_HEIGHT-1],
    input wire [DATA_WIDTH-1:0] matrix_b [0:CHUNK_WIDTH-1][0:CHUNK_HEIGHT-1],

    output wire [DATA_WIDTH*2-1:0] row_c [0:CHUNK_WIDTH-1]
);

    // reg [DATA_WIDTH*2-1:0] temp [0:CHUNK_WIDTH-1];

    // always @(posedge start) begin
    //     integer i, j;
    //     for (i = 0; i < CHUNK_HEIGHT; i++) begin
    //         for (j = 0; j < CHUNK_WIDTH; j++) begin
    //             temp[j] = temp[j] + row_a[i] * matrix_b[j][i];
    //         end
    //     end
    // end

    // assign row_c = temp;

    genvar r, c;

    generate
        for (c = 0; c < CHUNK_WIDTH; c = c + 1) begin
            // mmacc_dotprod #(DATA_WIDTH, CHUNK_WIDTH) dp (row_a, matrix_b[c], row_c[c]);
            // wire [DATA_WIDTH-1:0] mul_result [0:CHUNK_HEIGHT-1];

            // for (r = 0; r < CHUNK_HEIGHT; r = r + 1) begin
            //     mul_result[r] = row_a[r] * matrix_b[c][r];
            // end

            wire [DATA_WIDTH*2-1:0] psum [0:CHUNK_HEIGHT-1];

            assign psum[0] = row_a[0] * matrix_b[c][0];

            for (r = 1; r < CHUNK_HEIGHT; r = r + 1) begin
                assign psum[r] = psum[r-1] + row_a[r] * matrix_b[c][r];
            end

            assign row_c[c] = psum[CHUNK_HEIGHT-1];
        end
    endgenerate

endmodule


// dot product of two vectors
module mmacc_dotprod #(
    parameter DATA_WIDTH = 32,
    parameter CHUNK_WIDTH = 8
) (
    input wire [DATA_WIDTH-1:0] row_a [0:CHUNK_WIDTH-1],
    input wire [DATA_WIDTH-1:0] row_b [0:CHUNK_WIDTH-1],
    output reg [DATA_WIDTH*2-1:0] out
);

    always begin
        integer i;

        out = 0;

        for (i = 0; i < CHUNK_WIDTH; i = i + 1) begin
            out = out + row_a[i] * row_b[i];
        end
    end

endmodule
