import { Body, Controller, Get, Post } from '@nestjs/common';
import { AppService } from './app.service';

@Controller()
export class AppController {
  constructor(private readonly appService: AppService) {}


  @Post('add_reading')
  async create(@Body('path') pathData: any[]) {
    console.log(pathData);
    const createdPath = await this.appService.create(pathData);

  
    return { message: 'Paths created successfully', paths: createdPath };
  }

  @Get("get_all_readings")
  async findAll() {
    const paths = await this.appService.findAll();
    return { paths };
  }
}
